#!/usr/bin/env python3
import os
import sys
import json
import subprocess
import re


def mean(values):
    """Return the arithmetic mean of a sequence."""
    return sum(values) / len(values) if values else 0.0


def std(values):
    """Return the standard deviation of a sequence."""
    if not values:
        return 0.0
    m = mean(values)
    return (sum((x - m) ** 2 for x in values) / len(values)) ** 0.5

def load_config(bench_dir):
    """Return the parsed bench_config.json for the benchmark directory."""
    config_path = os.path.join(bench_dir, "bench_config.json")
    with open(config_path, "r") as f:
        return json.load(f)

def compile_and_run(bench_dir):
    """Compile the original and optimized sources and return executable paths."""
    if not os.path.exists(bench_dir):
        print("❌ Benchmark directory {} does not exist.".format(bench_dir))
        sys.exit(1)

    config = load_config(bench_dir)
    harness_file = "harness.cpp"
    extra_files = [src for src in config.get("sources", []) if src not in (harness_file, "original.cpp", "optimized.cpp")]

    executables = {}
    diag_files = {}
    for impl in ("original", "optimized"):
        exe_name = f"bench_{impl}"
        diag_file = f"compiler_{impl}.log"
        cmd = ["g++", harness_file, f"{impl}.cpp"] + extra_files + [
            "-o", exe_name,
            f"-fopt-info-all={diag_file}"
        ]
        for inc in config.get("includes", []):
            cmd += ["-I", inc]
        for lp in config.get("lib_paths", []):
            cmd += ["-L", lp]
        for lib in config.get("libs", []):
            cmd += [f"-l{lib}"]
        cmd += config.get("cxxflags", [])

        env_cxxflags = os.environ.get("CXXFLAGS")
        if env_cxxflags:
            cmd += env_cxxflags.split()

        print(f"⚙️ Compiling {impl} implementation into {exe_name} in {bench_dir}")
        subprocess.check_call(cmd, cwd=bench_dir)
        executables[impl] = os.path.join(bench_dir, exe_name)
        diag_files[impl] = diag_file

    return executables, diag_files

def run_executable(cmd):
    """Execute a compiled benchmark and parse float output values."""
    output = subprocess.check_output(cmd)
    tokens = output.decode().split()
    floats = []
    for t in tokens:
        try:
            floats.append(float(t))
        except ValueError:
            continue
    return floats

def measure_performance(exe, runs=10, timeout=None):
    """Run the executable multiple times and collect timing measurements.

    Parameters
    ----------
    exe : str
        Path to the executable.
    runs : int
        Number of executions to perform.
    timeout : float | None
        Maximum allowed seconds per run. The process is killed if it
        exceeds this limit and ``None`` is returned.
    """
    times = []
    for _ in range(runs):
        try:
            perf_output = subprocess.check_output(
                [exe, "--mode=perf"], timeout=timeout
            ).decode().splitlines()
        except subprocess.TimeoutExpired:
            print(f"⚠️ Benchmark run for {exe} exceeded {timeout} s and was terminated")
            return None
        try:
            time_val = float(perf_output[0].split()[1])
            times.append(time_val)
        except Exception as exc:
            print("⚠️ Could not parse performance output:", exc)
    return times

def parse_all_optimization_logs(log_text, file_filter=None):
    """Parse GCC optimization logs into a dictionary of detected passes."""
    optimization_patterns = {
        # GCC emits a variety of strings for the same optimization pass. Use
        # permissive regexes so we reliably detect them.
        "vectorization": r"loop vectorized",
        "loop_unrolling": r"loop unrolled",
        "inlining": r"inlining|inlined",
        "constant_propagation": r"propagat(e|ed).*const|constant propagation",
        "dead_code_elimination": r"eliminating dead code",
        "tail_call_optimization": r"tail call optimized",
        "strength_reduction": r"strength reduced",
        "loop_interchange": r"interchanged loop",
        "loop_peeling": r"peeled loop",
        "common_subexpr_elimination": r"common subexpression eliminated",
    }

    if file_filter:
        filtered_lines = [l for l in log_text.splitlines() if file_filter in l]
        log_text = "\n".join(filtered_lines)

    optimizations = {key: bool(re.search(pattern, log_text, re.I))
                     for key, pattern in optimization_patterns.items()}

    return optimizations

def write_summary(
    bench_dir,
    orig_mean,
    orig_std,
    opt_mean,
    opt_std,
    diff_pct,
    orig_times,
    opt_times,
    failed,
    new_opts,
    disabled_opts,
):
    """Write a short markdown summary for CI step outputs."""
    summary_path = os.getenv('GITHUB_STEP_SUMMARY')
    if summary_path:
        os.makedirs(os.path.dirname(summary_path), exist_ok=True)
        with open(summary_path, 'a') as f:
            f.write(f"## Benchmark Results: `{bench_dir}`\n\n")
            f.write("| Metric | Original (ms) | Optimized (ms) |\n")
            f.write("|--------|---------------|----------------|\n")
            f.write(f"| Mean   | {orig_mean:.2f} | {opt_mean:.2f} |\n")
            f.write(f"| Std    | {orig_std:.2f} | {opt_std:.2f} |\n\n")

            f.write("### Individual Runs (ms)\n")
            f.write("| Run | Original | Optimized |\n")
            f.write("|-----|----------|-----------|\n")
            for i, (o, opt) in enumerate(zip(orig_times, opt_times), start=1):
                f.write(f"| {i} | {o:.2f} | {opt:.2f} |\n")

            f.write("\n")
            if failed:
                if diff_pct < 0:
                    f.write(f"❌ **Benchmark failed**: optimized slower by {-diff_pct:.2f}%\n\n")
                else:
                    f.write(f"❌ **Benchmark failed**: {diff_pct:.2f}% difference (below threshold)\n\n")
            else:
                f.write(f"✅ **Benchmark passed**: {diff_pct:.2f}% improvement\n\n")

            if new_opts or disabled_opts:
                f.write("### Compiler Optimizations\n")
                if new_opts:
                    f.write("Newly enabled: " + ", ".join(sorted(new_opts)) + "\n")
                if disabled_opts:
                    f.write("Disabled: " + ", ".join(sorted(disabled_opts)) + "\n")
                f.write("\n")

            f.write("---\n\n")

def compare_outputs(bench_dir, executables, tol=1e-6, runs=10, min_improvement_pct=1.0,
                    new_opts=None, disabled_opts=None):
    """Check correctness and performance of the compiled benchmark."""
    print("\n=== Validation & Benchmark: {} ===".format(bench_dir))
    exe_orig = executables.get('original')
    exe_opt = executables.get('optimized')
    if not exe_orig or not exe_opt:
        print("❌ Missing bench executables.")
        sys.exit(1)

    print("-- Correctness Check --")
    orig_vals = run_executable([exe_orig, "--mode=correct"])
    opt_vals = run_executable([exe_opt, "--mode=correct"])
    if len(orig_vals) != len(opt_vals):
        print("❌ Shape mismatch: original {}, optimized {}".format(len(orig_vals), len(opt_vals)))
        sys.exit(1)
    diffs = [abs(a - b) for a, b in zip(orig_vals, opt_vals)]
    max_diff = max(diffs) if diffs else 0.0
    if max_diff >= tol:
        idx = diffs.index(max_diff)
        print("❌ Value mismatch at index {}: {} vs {} (Δ={})".format(idx, orig_vals[idx], opt_vals[idx], max_diff))
        sys.exit(1)
    print("✅ Correctness passed: outputs match within tolerance.")

    print("-- Performance Measurement ({} runs) --".format(runs))
    orig_times = measure_performance(exe_orig, runs)
    opt_times = measure_performance(exe_opt, runs)

    orig_mean, orig_std = mean(orig_times), std(orig_times)
    opt_mean, opt_std = mean(opt_times), std(opt_times)

    diff_ms = orig_mean - opt_mean
    diff_pct = (diff_ms / orig_mean) * 100 if orig_mean else 0

    # Fail if workload is too small or optimization does not provide the
    # required improvement. Negative diff_pct means the optimized version
    # is slower, which should also be treated as a failure.
    failed = orig_mean < 5000 or diff_pct < min_improvement_pct

    print("Original time:  mean = {:.2f} ms, std = {:.2f} ms".format(orig_mean, orig_std))
    print("Optimized time: mean = {:.2f} ms, std = {:.2f} ms".format(opt_mean, opt_std))

    if orig_mean < 5000:
        print("❌ Original runtime below 5000 ms. Increase workload.")

    if diff_pct >= min_improvement_pct:
        print("✅ Optimized faster by {:.2f} ms ({:.2f}%)".format(diff_ms, diff_pct))
    elif diff_pct <= -min_improvement_pct:
        print("❌ Optimized slower by {:.2f} ms ({:.2f}%)".format(-diff_ms, -diff_pct))
    else:
        print("❌ Difference below threshold ({:.2f}%)".format(diff_pct))

    write_summary(
        bench_dir,
        orig_mean,
        orig_std,
        opt_mean,
        opt_std,
        diff_pct,
        orig_times,
        opt_times,
        failed,
        new_opts or [],
        disabled_opts or [],
    )

    if failed:
        sys.exit(1)

def main():
    """Entry point when running this module as a script."""
    if len(sys.argv) < 2:
        print("Usage: validate.py <bench_dir> [<bench_dir> ...]")
        sys.exit(1)
    for bench_dir in sys.argv[1:]:
        executables, diag_files = compile_and_run(bench_dir)

        new_opts = []
        disabled_opts = []

        diag_orig = os.path.join(bench_dir, diag_files.get("original", ""))
        diag_opt = os.path.join(bench_dir, diag_files.get("optimized", ""))
        if os.path.exists(diag_orig) and os.path.exists(diag_opt):
            with open(diag_orig, "r") as f:
                log_orig = f.read()
            with open(diag_opt, "r") as f:
                log_opt = f.read()

            orig_opts = parse_all_optimization_logs(log_orig)
            opt_opts = parse_all_optimization_logs(log_opt)

            new_opts = [k for k in opt_opts if opt_opts[k] and not orig_opts.get(k, False)]
            disabled_opts = [k for k in orig_opts if orig_opts[k] and not opt_opts.get(k, False)]

            optimizations_detected = {
                "original": orig_opts,
                "optimized": opt_opts,
                "new": new_opts,
                "disabled": disabled_opts,
            }
            with open(os.path.join(bench_dir, "optimizations_detected.json"), "w") as f_json:
                json.dump(optimizations_detected, f_json, indent=2)

        compare_outputs(bench_dir, executables, new_opts=new_opts, disabled_opts=disabled_opts)

if __name__ == "__main__":
    main()