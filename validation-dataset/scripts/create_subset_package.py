#!/usr/bin/env python3
"""Package benchmarks and performance data by category.

This script copies original and optimized source files along with any LLM
outputs for each benchmark. The subset of ``benchmark_performance.json`` is
filtered using the categories defined in ``consolidated_benchmarks.json``.
"""

from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path
from typing import Dict, List


def load_categories(path: Path) -> Dict[str, List[str]]:
    """Return mapping of category -> list of benchmark names."""
    with path.open() as f:
        data = json.load(f)

    categories: Dict[str, List[str]] = {}
    for bench in data.get("benchmarks", []):
        name = bench.get("name")
        cat = bench.get("inefficiency") or "unknown"
        categories.setdefault(cat, []).append(name)
    return categories


def load_performance(path: Path) -> Dict:
    """Load the benchmark performance JSON."""
    with path.open() as f:
        return json.load(f)


def filter_performance(perf: Dict, allowed: set[str]) -> Dict:
    """Return a copy containing only benchmarks in ``allowed``."""
    benches = [b for b in perf.get("benchmarks", []) if b.get("benchmark_id") in allowed]
    return {**{k: v for k, v in perf.items() if k != "benchmarks"}, "benchmarks": benches}


def copy_if_exists(src: Path, dst: Path) -> None:
    if src.exists():
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(src, dst)


def copy_llm_outputs(src_root: Path, dest_root: Path) -> None:
    """Copy all ``*.cpp`` files from ``src_root`` to ``dest_root``."""
    if not src_root.is_dir():
        return
    dest_root.mkdir(parents=True, exist_ok=True)
    for file in src_root.glob("*.cpp"):
        shutil.copy(file, dest_root / file.name)


def main() -> None:
    parser = argparse.ArgumentParser(description="Package benchmark subset")
    parser.add_argument(
        "--benchmarks-json",
        default="consolidated_benchmarks.json",
        help="Benchmark metadata with categories",
    )
    parser.add_argument(
        "--performance-json",
        default="reports/benchmark_performance.json",
        help="Benchmark performance report",
    )
    parser.add_argument(
        "--benchmarks-dir", default="benchmarks", help="Directory with benchmarks"
    )
    parser.add_argument(
        "--llm-dir", default="llm_outputs", help="Directory containing LLM outputs"
    )
    parser.add_argument("output", help="Destination directory")
    parser.add_argument(
        "--model",
        default="gpt-4o-mini",
        help="LLM model subdirectory name",
    )

    args = parser.parse_args()

    categories = load_categories(Path(args.benchmarks_json))
    perf = load_performance(Path(args.performance_json))

    dest_root = Path(args.output)
    dest_root.mkdir(parents=True, exist_ok=True)

    # Copy code grouped by category
    all_benchmarks: set[str] = set()
    for cat, names in categories.items():
        safe_cat = cat.replace("/", "_")
        for name in names:
            all_benchmarks.add(name)
            src_dir = Path(args.benchmarks_dir) / name
            dest_dir = dest_root / safe_cat / name
            copy_if_exists(src_dir / "original.cpp", dest_dir / "original.cpp")
            copy_if_exists(src_dir / "optimized.cpp", dest_dir / "optimized.cpp")

            # Single step outputs
            single_src = (
                Path(args.llm_dir)
                / "downstream_task_1_optimized_code"
                / args.model
                / name
            )
            copy_llm_outputs(single_src, dest_dir / "llm_single")

            # Multi step outputs
            multi_src = (
                Path(args.llm_dir) / "multi_step_optimized_code" / args.model / name
            )
            copy_llm_outputs(multi_src, dest_dir / "llm_multi")

    subset = filter_performance(perf, all_benchmarks)
    with (dest_root / "benchmark_performance.json").open("w", encoding="utf-8") as f:
        json.dump(subset, f, indent=2)


if __name__ == "__main__":
    main()
