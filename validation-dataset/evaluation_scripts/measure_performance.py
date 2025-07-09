#!/usr/bin/env python3
"""Measure benchmark performance using existing validation utilities."""

# Small convenience wrapper around validate.measure_performance used for manual
# timing experiments.
import argparse
from validate import compile_and_run, measure_performance, mean, std


def main():
    """Compile the benchmark and print mean/std timing information."""
    parser = argparse.ArgumentParser(description="Measure benchmark performance")
    parser.add_argument("bench_dir", help="Benchmark directory")
    parser.add_argument("--runs", type=int, default=10, help="Number of runs")
    args = parser.parse_args()

    executables, _ = compile_and_run(args.bench_dir)
    exe_orig = executables.get("original")
    exe_opt = executables.get("optimized")
    if not exe_orig or not exe_opt:
        raise SystemExit("Missing benchmark executables")
    orig = measure_performance(exe_orig, args.runs)
    opt = measure_performance(exe_opt, args.runs)

    print("Original:  mean={:.2f} std={:.2f}".format(mean(orig), std(orig)))
    print("Optimized: mean={:.2f} std={:.2f}".format(mean(opt), std(opt)))


if __name__ == "__main__":
    main()
