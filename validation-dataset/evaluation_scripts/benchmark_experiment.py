#!/usr/bin/env python3
"""Evaluate multiple LLM solutions per benchmark and summarize results.

This tool can optionally query an LLM to generate candidate implementations
for each benchmark.  Every optimized candidate is compiled and executed
**exactly** ten times in order to collect mean and standard deviation timing
statistics.  Solution files follow the pattern ``optimized_<k>.cpp`` which
mirrors the output of ``generate_optimized_code.py``.  Results are written to
``reports/benchmark_performance.json`` so downstream tasks can analyse the
performance characteristics.
"""

from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path
from typing import List, Dict, Any
import platform
import os
import sys
import shutil
import time

# Ensure repository root is on the Python path for imports when run as a script
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

# Reuse helpers from the existing validation utilities
from evaluation_scripts.validate import (
    compile_and_run,
    measure_performance,
    mean,
    std,
)
from evaluation_scripts.run_llm_outputs import compile_with_llm
from llm_integration.llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import ensure_dir, read_text, write_text


def _parse_json_response(text: str) -> Dict[str, Any]:
    """Return parsed JSON object from LLM text output."""
    if not text or not text.strip():
        raise ValueError("empty response")

    import re

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        text = fenced.group(1)

    decoder = json.JSONDecoder()
    idx = text.find("{")
    while idx != -1:
        try:
            obj, _ = decoder.raw_decode(text, idx)
            return obj
        except json.JSONDecodeError:
            idx = text.find("{", idx + 1)
    raise json.JSONDecodeError("No valid JSON object found", text, 0)


def _read_function_signature(bench_dir: Path) -> str:
    cfg_path = bench_dir / "bench_config.json"
    if not cfg_path.exists():
        return ""
    try:
        with cfg_path.open() as f:
            cfg = json.load(f)
        return cfg.get("target_function_signature", "")
    except Exception:
        return ""


def _collect_source(bench_dir: Path) -> str:
    files = [
        p
        for p in bench_dir.iterdir()
        if p.suffix in {".cpp", ".cc", ".c", ".hpp", ".h"}
        and p.name not in {"optimized.cpp", "harness.cpp"}
    ]
    parts = []
    for p in sorted(files):
        parts.append(f"// {p.name}\n{read_text(p)}")
    return "\n\n".join(parts)


def generate_single_step_solutions(
    bench_dir: Path, sol_dir: Path, model: str, count: int
) -> None:
    ensure_dir(sol_dir)
    prompt_file = Path("llm_integration") / "prompts" / f"{model}.json"
    if prompt_file.exists():
        with prompt_file.open() as f:
            data = json.load(f)
    else:
        data = {
            "prompt": "Optimize the following C++ code:\n\n<original_code>",
            "model": model,
        }
    template = data["prompt"]
    func_sig = _read_function_signature(bench_dir)
    code = _collect_source(bench_dir)
    for i in range(1, count + 1):
        prompt = template.replace("<original_code>", code)
        if "<function_signature>" in prompt:
            prompt = prompt.replace("<function_signature>", func_sig)
        try:
            resp = call_llm(prompt, model=data.get("model", model))
            result = _parse_json_response(resp)
            optimized = result["optimized_code"]
        except Exception as exc:
            write_text(sol_dir / f"optimized_{i}_error.txt", str(exc))
            continue
        out_file = sol_dir / f"optimized_{i}.cpp"
        write_text(out_file, optimized)


def generate_multistep_solutions(
    bench_dir: Path, sol_dir: Path, model: str, steps: int
) -> None:
    from llm_integration.multi_step_optimize import optimize_benchmark

    ensure_dir(sol_dir)
    optimize_benchmark(bench_dir, model, steps, runs=5, use_perf=False)
    src_root = Path("llm_outputs") / "multi_step_optimized_code" / model / bench_dir.name
    for i in range(1, steps + 1):
        step_file = src_root / f"step_{i}.cpp"
        if step_file.exists():
            shutil.copy(step_file, sol_dir / f"optimized_{i}.cpp")
    # Also copy the overall best candidate for easy evaluation
    best_file = src_root / "best.cpp"
    if best_file.exists():
        dst = sol_dir / "best.cpp"
        try:
            if best_file.resolve() != dst.resolve():
                shutil.copy(best_file, dst)
        except shutil.SameFileError:
            # Ignore attempts to copy a file onto itself
            pass


def get_system_info() -> Dict[str, Any]:
    info = {
        "cpu_count": os.cpu_count(),
        "cpu_model": platform.processor() or platform.machine(),
    }
    mem = None
    try:
        with open("/proc/meminfo") as f:
            for line in f:
                if line.startswith("MemTotal:"):
                    parts = line.split()
                    mem = round(int(parts[1]) / 1024**2, 2)
                    break
    except Exception:
        pass
    info["memory_gb"] = mem
    try:
        output = subprocess.check_output(["lscpu"], text=True)
        for line in output.splitlines():
            if "L3 cache:" in line:
                info["L3_cache"] = line.split(":", 1)[1].strip()
    except Exception:
        pass
    return info


def wait_for_benchmark_processes() -> None:
    """Block until no other benchmark executables are running."""
    while True:
        try:
            output = subprocess.check_output(["ps", "-eo", "pid,comm"], text=True)
        except Exception:
            return
        lines = [l.strip() for l in output.splitlines()[1:] if "bench_" in l]
        if not lines:
            break
        print("Waiting for other benchmark processes to finish:")
        for l in lines:
            print("  ", l)
        time.sleep(1)

def compile_solution(bench_dir: Path, src: Path, exe_name: str) -> Path | None:
    """Compile ``src`` with the benchmark harness.

    Returns the path to the executable on success, otherwise ``None``.
    """
    try:
        exe, _ = compile_with_llm(bench_dir, src.resolve(), exe_name)
        return exe
    except Exception:
        return None


def evaluate_executable(exe: Path, runs: int, timeout: float | None = None) -> Dict[str, Any] | None:
    """Return timing metrics and all run values for ``exe``."""
    try:
        times = measure_performance(str(exe), runs, timeout=timeout)
    except subprocess.CalledProcessError:
        return None
    if not times:
        return None
    return {"mean": mean(times), "std": std(times), "runs": times}


def evaluate_benchmark(
    bench_dir: Path,
    sol_dir: Path,
    runs: int,
    ks: List[int],
    model: str,
    generate: bool,
    multistep: int,
) -> Dict[str, Any]:
    """Return performance data for one benchmark."""
    if generate:
        if multistep > 0:
            generate_multistep_solutions(bench_dir, sol_dir, model, multistep)
        else:
            generate_single_step_solutions(bench_dir, sol_dir, model, max(ks))

    try:
        executables, _ = compile_and_run(str(bench_dir))
    except (FileNotFoundError, subprocess.CalledProcessError):
        return {
            "benchmark_id": bench_dir.name,
            "original_performance": None,
            "human_performance": None,
            "llm_solutions": [],
        }

    orig_perf = evaluate_executable(Path(executables["original"]), runs)
    human_perf = evaluate_executable(Path(executables["optimized"]), runs)
    if orig_perf is None or human_perf is None:
        return {
            "benchmark_id": bench_dir.name,
            "original_performance": orig_perf,
            "human_performance": human_perf,
            "llm_solutions": [],
        }

    solutions: List[Dict[str, Any]] = []
    timeout = None
    if orig_perf is not None:
        timeout = orig_perf.get("mean", 0) / 1000.0 + 3.0
    for k in ks:
        # In multi-step mode ``k`` typically selects the number of trials
        # performed. The best candidate from those trials is stored as
        # ``best.cpp``. When evaluating ``k == 1`` we therefore compile this
        # best version instead of the first step file so users can easily
        # measure the final optimized result.
        if multistep > 0 and k == 1:
            solution_path = sol_dir / "best.cpp"
        else:
            solution_path = sol_dir / f"optimized_{k}.cpp"
        solution_id = f"{bench_dir.name}_{k}"

        if not solution_path.exists():
            solutions.append({"solution_id": solution_id, "runnable": False, "performance": None})
            continue

        exe = compile_solution(bench_dir, solution_path, f"bench_llm_{k}")
        if not exe:
            solutions.append({"solution_id": solution_id, "runnable": False, "performance": None})
            continue

        perf = evaluate_executable(exe, runs, timeout=timeout)
        if perf is None:
            solutions.append({"solution_id": solution_id, "runnable": False, "performance": None})
        else:
            solutions.append({"solution_id": solution_id, "runnable": True, "performance": perf})

    return {
        "benchmark_id": bench_dir.name,
        "original_performance": orig_perf,
        "human_performance": human_perf,
        "llm_solutions": solutions,
    }


def load_mapping(path: Path) -> List[Dict[str, str]]:
    with path.open() as f:
        data = json.load(f)
    return data.get("benchmarks", [])


def write_single_benchmark(record: dict, model: str, approach: str) -> None:
    """Write per-benchmark results including all run timings."""
    out_dir = Path("reports") / model / approach
    out_dir.mkdir(parents=True, exist_ok=True)
    path = out_dir / f"{record['benchmark_id']}.json"
    with path.open("w", encoding="utf-8") as f:
        json.dump(record, f, indent=2)


def main() -> None:
    parser = argparse.ArgumentParser(description="Run benchmark experiment")
    parser.add_argument(
        "solutions_root",
        help="Directory containing per-benchmark solution files",
    )
    parser.add_argument(
        "benchmarks",
        nargs="*",
        help="Optional list of benchmark directories to run",
    )
    parser.add_argument(
        "--benchmarks-json",
        default="existing_benchmarks.json",
        help="Benchmark metadata mapping",
    )
    parser.add_argument("--runs", type=int, default=10, help="Executions per measurement")
    parser.add_argument(
        "-k",
        "--max-k",
        type=int,
        default=5,
        help="Evaluate solutions optimized_1.cpp through optimized_<k>.cpp",
    )
    parser.add_argument(
        "--output",
        default="reports/benchmark_performance.json",
        help="Output JSON file",
    )
    parser.add_argument(
        "--model",
        default="gpt-4o-mini",
        help="LLM model name for generation",
    )
    parser.add_argument(
        "--generate",
        action="store_true",
        help="Generate solutions via LLM instead of using existing files",
    )
    parser.add_argument(
        "--multi-step",
        type=int,
        default=0,
        help="Number of refinement steps for multi-step optimization",
    )
    args = parser.parse_args()

    out_path = Path(args.output)

    # Load any previously saved results so completed benchmarks are not
    # re-evaluated. If the file cannot be parsed we simply start fresh.
    existing_results: list[dict] = []
    completed = set()
    if out_path.exists():
        try:
            with out_path.open() as f:
                data = json.load(f)
            existing_results = data.get("benchmarks", [])
            for item in existing_results:
                bid = item.get("benchmark_id")
                if bid:
                    completed.add(bid)
        except Exception:
            existing_results = []
            completed = set()

    if args.benchmarks:
        bench_entries = [
            {
                "name": Path(b).name,
                "inefficiency": None,
                "optimization": None,
                "path": Path(b) if Path(b).is_dir() else Path("benchmarks") / b,
            }
            for b in args.benchmarks
        ]
    else:
        bench_entries = []
        for b in load_mapping(Path(args.benchmarks_json)):
            if isinstance(b, dict):
                bench_entries.append(
                    {
                        "name": b.get("name"),
                        "inefficiency": b.get("inefficiency"),
                        "optimization": b.get("optimization"),
                        "path": Path("benchmarks") / b.get("name", ""),
                    }
                )
            else:
                bench_entries.append(
                    {
                        "name": Path(b).name,
                        "inefficiency": None,
                        "optimization": None,
                        "path": Path("benchmarks") / Path(b).name,
                    }
                )

    results = list(existing_results)
    sol_root = Path(args.solutions_root)
    ks = list(range(1, args.max_k + 1))

    parent = sol_root.resolve().parent
    if parent.name and parent.name != "llm_outputs":
        approach = parent.name.replace("_optimized_code", "")
    else:
        approach = "multi_step" if args.multi_step > 0 else "single_step"
    system_info = get_system_info()

    for bench in bench_entries:
        bench_id = bench.get("name")
        bench_dir: Path = bench.get("path")  # type: ignore
        if not bench_id or not bench_dir:
            continue
        if bench_id in completed:
            print(f"Skipping {bench_id} (already evaluated)")
            continue
        wait_for_benchmark_processes()
        sol_dir = sol_root / bench_dir.name
        if not bench_dir.is_dir():
            continue
        if not sol_dir.is_dir():
            if args.generate:
                sol_dir.mkdir(parents=True, exist_ok=True)
            else:
                continue
        record = evaluate_benchmark(
            bench_dir,
            sol_dir,
            args.runs,
            ks,
            args.model,
            args.generate,
            args.multi_step,
        )
        record["category"] = bench.get("inefficiency")
        record["optimization_type"] = bench.get("optimization")
        results.append(record)
        write_single_benchmark(record, args.model, approach)
        wait_for_benchmark_processes()

        out_path.parent.mkdir(parents=True, exist_ok=True)
        summary = {
            "model": args.model,
            "approach": approach,
            "system": system_info,
            "benchmarks": results,
        }
        with out_path.open("w", encoding="utf-8") as f:
            json.dump(summary, f, indent=2)
        print(f"Updated {out_path} with {bench_id}")



if __name__ == "__main__":
    main()

