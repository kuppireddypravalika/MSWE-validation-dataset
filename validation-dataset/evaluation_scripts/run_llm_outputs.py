#!/usr/bin/env python3
"""Run benchmarks using LLM-generated optimized code with detailed performance summary, correctness checks, comprehensive compiler optimization reporting, and JSON report generation."""

# This script is primarily used to evaluate code produced by an external LLM.
# It compiles the generated implementation, compares it with the baseline, and
# emits human-readable as well as machine-readable reports.
import argparse
import os
import subprocess
import json
from pathlib import Path
from datetime import datetime
import math
import re
import sys

# Ensure repository root is on the Python path so we can import from scripts/
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from scripts.gcc_pass_parser import extract_gcc_passes
from scripts.optimization_counter import count_from_gcc_log


def _remove_gcc_dumps(directory: Path) -> None:
    """Delete GCC dump files in the given directory."""
    # Dump filenames can include hyphens (e.g. `ipa-clones`). Include '-' in
    # the final component so we clean up all generated files.
    pattern = re.compile(r".*\.\d+[a-z]+\.[A-Za-z0-9_-]+$")
    for entry in directory.iterdir():
        if pattern.match(entry.name):
            try:
                entry.unlink()
            except OSError:
                pass


# Import helpers from the ``evaluation_scripts`` package. When this module is
# imported (e.g. by ``llm_integration/multi_step_optimize.py``) the current
# working directory may not include ``evaluation_scripts`` in ``sys.path``. Use
# an explicit package import so the module resolves correctly in all cases.
from evaluation_scripts.validate import (
    load_config,
    compile_and_run,
    measure_performance,
    mean,
    std,
    run_executable,
    parse_all_optimization_logs,
)


from typing import Tuple


def compile_with_llm(bench_dir: Path, llm_file: Path, exe_name: str) -> Tuple[Path, Path]:
    """Compile harness with the LLM optimized implementation."""
    config = load_config(bench_dir)
    harness_file = "harness.cpp"
    diag_file = f"{exe_name}_optimizations.log"

    extra_files = [
        src
        for src in config.get("sources", [])
        if src not in (harness_file, "original.cpp", "optimized.cpp")
    ]

    cmd = ["g++", harness_file, str(llm_file)] + extra_files + [
        "-o",
        exe_name,
        f"-fopt-info-all={diag_file}",
    ]
    # Always add the benchmark directory itself to the include search path.
    # LLM generated files may live outside the benchmark folder and include
    # headers that reside alongside the original sources (e.g. common.h).
    cmd += ["-I", str(bench_dir.resolve())]
    cmd += ["-I" + inc for inc in config.get("includes", [])]
    cmd += ["-L" + lp for lp in config.get("lib_paths", [])]
    cmd += [f"-l{lib}" for lib in config.get("libs", [])]
    cmd += config.get("cxxflags", [])

    env_cxxflags = os.environ.get("CXXFLAGS")
    if env_cxxflags:
        cmd += env_cxxflags.split()

    print(f"⚙️  Compiling LLM optimized benchmark '{exe_name}' in {bench_dir}")

    try:
        subprocess.check_call(cmd, cwd=bench_dir)
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Compilation failed: {e}")

    exe_path = bench_dir / exe_name
    diag_path = bench_dir / diag_file
    return exe_path, diag_path


def run_benchmark(exe: Path, runs: int):
    """Return timing measurements for the given executable."""
    return measure_performance(str(exe), runs)


def generate_json_report(
    report_dir: Path,
    model: str,
    benchmark: str,
    timings: dict,
    correctness_passed: bool,
    optimizations: dict,
    passes: dict,
    opt_counts: dict,
    failure_message: str | None = None,
):
    """Write a JSON file capturing performance numbers and metadata."""
    report = {
        "model": model,
        "benchmark": benchmark,
        "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "performance": timings,
        "correctness_passed": correctness_passed,
        "compiler_optimizations": optimizations,
        "gcc_passes": passes,
        "optimization_counts": opt_counts,
        "failure": failure_message,
    }

    json_path = report_dir / model / f"{benchmark}.json"
    json_path.parent.mkdir(parents=True, exist_ok=True)

    with json_path.open("w") as f:
        json.dump(report, f, indent=4)


def generate_markdown_report(
    report_dir: Path,
    model: str,
    benchmark: str,
    timings: dict,
    correctness_passed: bool,
    optimization_table: dict,
    individual_runs: list,
    gcc_passes: dict,
    opt_counts: dict,
    failure_message: str | None = None,
):
    """Emit a Markdown summary table for humans to read.

    Parameters
    ----------
    report_dir : Path
        Directory where the markdown file will be written.
    model : str
        Model name used for the benchmark.
    benchmark : str
        Name of the benchmark.
    timings : dict
        Aggregated timing statistics.
    correctness_passed : bool
        Whether the LLM implementation matched the baseline output.
    optimization_table : dict
        Table of high level optimization flags enabled for each build.
    individual_runs : list
        List of per-run timing measurements.
    gcc_passes : dict
        GCC pass frequency information with ``original``, ``baseline`` and ``llm`` keys.
    failure_message : str | None, optional
        Error string if the benchmark failed.
    """
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    report_path = report_dir / model / f"{benchmark}.md"
    report_path.parent.mkdir(parents=True, exist_ok=True)

    with report_path.open("w") as f:
        f.write(f"# Benchmark Report\n")
        f.write(f"- **Model**: {model}\n")
        f.write(f"- **Benchmark**: {benchmark}\n")
        f.write(f"- **Timestamp**: {timestamp}\n")

        f.write("\n## Performance Summary\n")
        f.write("| Version | Mean (ms) | Std (ms) | Speedup vs Original | Speedup vs Baseline Optimized |\n")
        f.write("|---------|-----------|----------|---------------------|-------------------------------|\n")
        for version in ["original", "baseline_optimized", "llm_optimized"]:
            mean_v = timings[version]['mean']
            std_v = timings[version]['std']
            if version != "original" and mean_v and not math.isnan(mean_v):
                speedup_orig_val = timings["original"]["mean"] / mean_v
                speedup_orig = f"{speedup_orig_val:.2f}x"
            else:
                speedup_orig = "N/A"
            if version == "llm_optimized" and mean_v and not math.isnan(mean_v):
                speedup_base_val = timings["baseline_optimized"]["mean"] / mean_v
                speedup_base = f"{speedup_base_val:.2f}x"
            else:
                speedup_base = "N/A"
            f.write(
                f"| {version.replace('_', ' ').title()} | {mean_v:.2f} | {std_v:.2f} | {speedup_orig} | {speedup_base} |\n"
            )

        f.write("\n## Correctness Check\n")
        f.write(f"{'✅ Passed' if correctness_passed else '❌ Failed'}\n")

        f.write("\n## Compiler Optimization Passes\n")
        f.write("| Optimization Pass | Original | Baseline Optimized | LLM Optimized |\n")
        f.write("|-------------------|----------|--------------------|---------------|\n")
        for opt, states in optimization_table.items():
            f.write(f"| {opt.replace('_', ' ').capitalize()} | {'✅' if states['original'] else '❌'} | {'✅' if states['baseline'] else '❌'} | {'✅' if states['llm'] else '❌'} |\n")

        # Explicit transformation counts
        if opt_counts:
            f.write("\n## Optimization Transformation Counts\n")
            f.write("| Metric | Original | Baseline Optimized | LLM Optimized | Base-Orig | LLM-Base |\n")
            f.write("|-------|---------|-------------------|--------------|----------|---------|\n")
            metrics = sorted(opt_counts.get('original', {}).keys())
            for m in metrics:
                orig_c = opt_counts['original'].get(m, 0)
                base_c = opt_counts['baseline'].get(m, 0)
                llm_c = opt_counts['llm'].get(m, 0)
                diff_b = base_c - orig_c
                diff_l = llm_c - base_c
                f.write(f"| {m} | {orig_c} | {base_c} | {llm_c} | {diff_b:+d} | {diff_l:+d} |\n")

        # GCC optimization pass counts comparison
        orig_passes = gcc_passes.get("original", {})
        baseline_passes = gcc_passes.get("baseline", {})
        llm_passes = gcc_passes.get("llm", {})
        all_passes = sorted(set(orig_passes) | set(baseline_passes) | set(llm_passes))

        if all_passes:
            f.write("\n## GCC Pass Count Comparison\n")
            f.write("| Pass | Original Count | Baseline Count | LLM Count | Base-Orig | LLM-Base |\n")
            f.write("|------|---------------|---------------|-----------|-----------|----------|\n")
            for p in all_passes:
                orig_cnt = orig_passes.get(p, {}).get("count", 0)
                base_cnt = baseline_passes.get(p, {}).get("count", 0)
                llm_cnt = llm_passes.get(p, {}).get("count", 0)
                diff_base = base_cnt - orig_cnt
                diff_llm = llm_cnt - base_cnt
                f.write(f"| {p} | {orig_cnt} | {base_cnt} | {llm_cnt} | {diff_base:+d} | {diff_llm:+d} |\n")

        f.write("\n## Individual Run Timings (ms)\n")
        f.write("| Run | Original | Baseline Optimized | LLM Optimized |\n")
        f.write("|-----|----------|--------------------|---------------|\n")
        for i, run in enumerate(individual_runs):
            f.write(
                f"| {i+1} | {run['original']:.2f} | {run['baseline']:.2f} | {run['llm']:.2f} |\n"
            )

        if failure_message:
            f.write("\n## Error\n")
            f.write(f"{failure_message}\n")


def main():
    """Entry point for evaluating LLM generated benchmarks."""
    parser = argparse.ArgumentParser(description="Execute LLM optimized benchmark(s)")
    parser.add_argument("llm_dir", help="Path to LLM output directory. If --benchmark-file is provided, this should point to the model directory containing benchmark subdirectories.")
    parser.add_argument("--runs", type=int, default=10, help="Number of runs")
    parser.add_argument("--benchmark-file", help="JSON file listing benchmark names to evaluate")
    args = parser.parse_args()

    benchmarks = []
    if args.benchmark_file:
        bench_file = Path(args.benchmark_file)
        if not bench_file.is_file():
            raise SystemExit(f"Benchmark file not found: {bench_file}")
        with bench_file.open() as f:
            data = json.load(f)
            if isinstance(data, dict) and "benchmarks" in data:
                benchmarks = [b["name"] if isinstance(b, dict) else b for b in data["benchmarks"]]
            elif isinstance(data, list):
                benchmarks = [b["name"] if isinstance(b, dict) else b for b in data]
            else:
                raise SystemExit("Benchmark file format not recognized")
    else:
        benchmarks = [Path(args.llm_dir).name]

    llm_root = Path(args.llm_dir).resolve()

    for benchmark in benchmarks:
        llm_dir = llm_root / benchmark if args.benchmark_file else llm_root
        llm_file = llm_dir / "optimized.cpp"

        if not llm_file.exists():
            print(f"⚠️  LLM optimized file not found: {llm_file}. Skipping.")
            continue

        model = llm_dir.parent.name
        bench_dir = Path("benchmarks") / benchmark

        if not bench_dir.exists():
            raise SystemExit(f"Benchmark directory not found: {bench_dir}")

        dump_flags = "-fdump-tree-all-all -fdump-rtl-all-all -fdump-ipa-all-all"
        old_cxxflags = os.environ.get("CXXFLAGS", "")
        os.environ["CXXFLAGS"] = f"{old_cxxflags} {dump_flags}".strip()
        _remove_gcc_dumps(bench_dir)
        _remove_gcc_dumps(llm_file.parent)

        try:
            baseline_execs, diag_files = compile_and_run(str(bench_dir))
        except subprocess.CalledProcessError as e:
            print(
                f"❌ Benchmark '{benchmark}' failed during baseline compilation: {e}"
            )
            continue

        # Collect GCC pass counts for both the original and baseline optimized
        # compilations separately. This allows comparing all three builds
        # against each other later on.
        orig_passes = extract_gcc_passes(str(bench_dir), "original.cpp")
        baseline_passes = extract_gcc_passes(str(bench_dir), "optimized.cpp")
        _remove_gcc_dumps(bench_dir)

        base_orig_times = run_benchmark(Path(baseline_execs["original"]), args.runs)
        base_opt_times = run_benchmark(Path(baseline_execs["optimized"]), args.runs)

        correctness_passed = True
        individual_runs = []
        optimization_table = {}
        failure_message = None
        llm_opt_times = []

        try:
            llm_exe, llm_diag = compile_with_llm(bench_dir, llm_file, "bench_llm")
        except RuntimeError as e:
            failure_message = str(e)
            llm_exe = None
            llm_diag = None
        llm_passes = {}
        if llm_exe:
            # GCC dump files are emitted relative to the current working
            # directory used during compilation (the benchmark folder). Parse
            # dumps from ``bench_dir`` so we actually capture the LLM pass data.
            llm_passes = extract_gcc_passes(str(bench_dir), llm_file.name)
        _remove_gcc_dumps(bench_dir)
        _remove_gcc_dumps(llm_file.parent)

        # Perform correctness and performance checks only if compilation succeeded
        if llm_exe:
            try:
                orig_vals = run_executable([baseline_execs["original"], "--mode=correct"])
                llm_vals = run_executable([llm_exe, "--mode=correct"])
                correctness_passed = all(abs(o - l) < 1e-6 for o, l in zip(orig_vals, llm_vals))
            except subprocess.CalledProcessError as e:
                failure_message = f"Execution failed: {e}"
                correctness_passed = False

            if failure_message is None:
                try:
                    llm_opt_times = run_benchmark(llm_exe, args.runs)
                except subprocess.CalledProcessError as e:
                    failure_message = f"Performance run failed: {e}"
                    llm_opt_times = []
        else:
            correctness_passed = False

        # Parse compiler optimization logs
        with open(bench_dir / diag_files["original"], "r") as f:
            log_orig = f.read()
        with open(bench_dir / diag_files["optimized"], "r") as f:
            log_opt = f.read()
        orig_opts = parse_all_optimization_logs(log_orig)
        base_opts = parse_all_optimization_logs(log_opt)
        orig_counts = count_from_gcc_log(log_orig)
        base_counts = count_from_gcc_log(log_opt)
        llm_opts = {}
        llm_counts = {k: 0 for k in orig_counts}
        if llm_diag and os.path.exists(llm_diag):
            with open(llm_diag, "r") as f:
                log_llm = f.read()
            llm_opts = parse_all_optimization_logs(log_llm)
            llm_counts = count_from_gcc_log(log_llm)

        # Build optimization table
        all_opts = set(orig_opts.keys()).union(base_opts.keys(), llm_opts.keys())
        for opt in all_opts:
            optimization_table[opt] = {
                "original": orig_opts.get(opt, False),
                "baseline": base_opts.get(opt, False),
                "llm": llm_opts.get(opt, False),
            }

        # Prepare individual run timings
        if not llm_opt_times:
            llm_fill = [float("nan")] * len(base_orig_times)
        else:
            llm_fill = llm_opt_times
        individual_runs = [
            {"original": o, "baseline": b, "llm": l}
            for o, b, l in zip(base_orig_times, base_opt_times, llm_fill)
        ]

        timings = {
            "original": {"mean": mean(base_orig_times), "std": std(base_orig_times)},
            "baseline_optimized": {"mean": mean(base_opt_times), "std": std(base_opt_times)},
            "llm_optimized": {
                "mean": mean(llm_opt_times) if llm_opt_times else float("nan"),
                "std": std(llm_opt_times) if llm_opt_times else float("nan"),
            },
        }

        # Generate reports
        report_dir = Path("reports")
        generate_markdown_report(
            report_dir,
            model,
            benchmark,
            timings,
            correctness_passed,
            optimization_table,
            individual_runs,
            {"original": orig_passes, "baseline": baseline_passes, "llm": llm_passes},
            {"original": orig_counts, "baseline": base_counts, "llm": llm_counts},
            failure_message,
        )
        generate_json_report(
            report_dir,
            model,
            benchmark,
            timings,
            correctness_passed,
            optimization_table,
            {"original": orig_passes, "baseline": baseline_passes, "llm": llm_passes},
            {"original": orig_counts, "baseline": base_counts, "llm": llm_counts},
            failure_message,
        )

        if failure_message:
            print(f"❌ Benchmark '{benchmark}' encountered an error: {failure_message}")
        else:
            print(
                f"✅ Reports successfully generated at {report_dir}/{model}/{benchmark}.md and .json"
            )

        if old_cxxflags:
            os.environ["CXXFLAGS"] = old_cxxflags
        else:
            os.environ.pop("CXXFLAGS", None)


if __name__ == "__main__":
    main()
