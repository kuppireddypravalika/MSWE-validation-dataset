#!/usr/bin/env python3
"""Utility script to compile and optionally run a benchmark."""

# This helper is useful when iterating on a single benchmark. It mirrors the
# logic from validate.py but exposes a simpler interface for quick testing.
import argparse
from validate import compile_and_run, run_executable


def main():
    """Parse arguments and invoke compilation and execution."""
    parser = argparse.ArgumentParser(description="Compile and run a benchmark")
    parser.add_argument("bench_dir", help="Benchmark directory")
    parser.add_argument("--mode", choices=["correct", "perf"], default="correct")
    parser.add_argument("--impl", choices=["original", "optimized"], default="original")
    args = parser.parse_args()

    executables, _ = compile_and_run(args.bench_dir)
    exe = executables.get(args.impl)
    if not exe:
        raise SystemExit(f"Missing executable for {args.impl}")
    output = run_executable([exe, f"--mode={args.mode}"])
    print(" ".join(str(v) for v in output))


if __name__ == "__main__":
    main()
