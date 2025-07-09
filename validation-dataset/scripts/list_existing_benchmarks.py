#!/usr/bin/env python3
"""Generate a JSON list of benchmarks containing a harness.cpp file."""

# Useful for quickly enumerating all available benchmarks. The resulting JSON is
# consumed by some of the automation scripts.
import argparse
import json
from pathlib import Path


def main() -> None:
    """Scan the benchmark directory tree and write out a JSON manifest."""
    parser = argparse.ArgumentParser(
        description="List benchmark directories that contain harness.cpp"
    )
    parser.add_argument(
        "--bench-root", default="benchmarks", help="Root directory of benchmarks"
    )
    parser.add_argument(
        "--output", default="existing_benchmarks.json", help="Output JSON file"
    )
    args = parser.parse_args()

    bench_root = Path(args.bench_root)
    benchmarks = [
        p.name
        for p in sorted(bench_root.glob("benchmark_*"))
        if (p / "harness.cpp").is_file()
    ]

    with open(args.output, "w", encoding="utf-8") as f:
        json.dump({"benchmarks": benchmarks}, f, indent=2)

    print(f"Found {len(benchmarks)} benchmarks. Wrote {args.output}")


if __name__ == "__main__":
    main()
