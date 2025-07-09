#!/usr/bin/env python3
"""Create per-category benchmark_performance.json files."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def load_mapping(path: Path) -> dict[str, list[str]]:
    """Return mapping of category -> benchmark names."""
    with path.open() as f:
        data = json.load(f)
    categories: dict[str, list[str]] = {}
    for bench in data.get("benchmarks", []):
        name = bench.get("name")
        cat = bench.get("inefficiency", "misc")
        if name:
            categories.setdefault(cat, []).append(name)
    return categories


def filter_perf(perf: dict, names: set[str]) -> dict:
    """Return performance JSON filtered to selected benchmarks."""
    subset = {k: v for k, v in perf.items() if k != "benchmarks"}
    subset["benchmarks"] = [
        b for b in perf.get("benchmarks", []) if b.get("benchmark_id") in names
    ]
    return subset


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create per-category benchmark performance subsets"
    )
    parser.add_argument(
        "--benchmarks-json",
        default="consolidated_benchmarks.json",
        help="Benchmark metadata mapping benchmarks to categories",
    )
    parser.add_argument(
        "--performance-json",
        default="category_packages/benchmark_performance.json",
        help="Combined benchmark performance JSON",
    )
    parser.add_argument(
        "--output-dir", default="category_packages", help="Directory with categories"
    )
    args = parser.parse_args()

    categories = load_mapping(Path(args.benchmarks_json))
    with Path(args.performance_json).open() as f:
        perf = json.load(f)

    for cat, names in categories.items():
        safe_cat = cat.replace("/", "_")
        out_dir = Path(args.output_dir) / safe_cat
        out_dir.mkdir(parents=True, exist_ok=True)
        subset = filter_perf(perf, set(names))
        out_file = out_dir / "benchmark_performance.json"
        with out_file.open("w", encoding="utf-8") as f:
            json.dump(subset, f, indent=2)


if __name__ == "__main__":
    main()
