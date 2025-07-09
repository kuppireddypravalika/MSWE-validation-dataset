#!/usr/bin/env python3
"""Generate CSV reports for benchmark results."""

# This script aggregates performance statistics across multiple benchmarks and
# writes them to a single CSV file for easier analysis.
import argparse
import csv
from pathlib import Path
from validate import compile_and_run, measure_performance, mean, std


def run_benchmark(bench_dir: str, runs: int):
    """Compile a benchmark and return timing statistics."""
    executables, _ = compile_and_run(bench_dir)
    exe_orig = executables.get("original")
    exe_opt = executables.get("optimized")
    if not exe_orig or not exe_opt:
        raise RuntimeError("Missing benchmark executables")
    orig = measure_performance(exe_orig, runs)
    opt = measure_performance(exe_opt, runs)
    return {
        "benchmark": Path(bench_dir).name,
        "orig_mean": mean(orig),
        "orig_std": std(orig),
        "opt_mean": mean(opt),
        "opt_std": std(opt),
        "diff_pct": ((mean(orig) - mean(opt)) / mean(orig) * 100) if orig else 0,
    }


def main():
    """Parse arguments, run benchmarks and emit the final CSV report."""
    parser = argparse.ArgumentParser(description="Generate CSV report for benchmarks")
    parser.add_argument("benchmarks", nargs="+", help="Benchmark directories")
    parser.add_argument("--runs", type=int, default=10)
    parser.add_argument("--output", default="reports/benchmark_results.csv")
    args = parser.parse_args()

    results = [run_benchmark(b, args.runs) for b in args.benchmarks]
    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    with open(out_path, "w", newline="") as f:
        writer = csv.DictWriter(
            f,
            fieldnames=["benchmark", "orig_mean", "orig_std", "opt_mean", "opt_std", "diff_pct"],
        )
        writer.writeheader()
        writer.writerows(results)
    print(f"Report written to {out_path}")


if __name__ == "__main__":
    main()
