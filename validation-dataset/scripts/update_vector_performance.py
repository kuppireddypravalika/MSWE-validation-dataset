#!/usr/bin/env python3
"""Update vector benchmarks and extract their performance.

This script removes any benchmarks listed in ``vector.json`` whose
``original_performance`` entry is ``null`` in ``reports/benchmark_performance.json``.
The updated vector list is written back to ``vector.json``. The remaining
benchmark entries are extracted from the performance report and stored in a
separate JSON file.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def load_json(path: Path) -> dict:
    with path.open() as f:
        return json.load(f)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Update vector.json and export matching performance data"
    )
    parser.add_argument(
        "--vector-json", default="vector.json", help="Path to vector.json"
    )
    parser.add_argument(
        "--performance-json",
        default="reports/benchmark_performance.json",
        help="Benchmark performance report",
    )
    parser.add_argument(
        "--output-json",
        default="reports/vector_benchmark_performance.json",
        help="Destination for filtered performance data",
    )
    args = parser.parse_args()

    vector_path = Path(args.vector_json)
    perf_path = Path(args.performance_json)
    output_path = Path(args.output_json)

    vector = load_json(vector_path)
    perf = load_json(perf_path)

    id_to_entry = {b.get("benchmark_id"): b for b in perf.get("benchmarks", [])}

    filtered_ids: list[str] = []
    for bench_id in vector.get("benchmarks", []):
        entry = id_to_entry.get(bench_id)
        if entry and entry.get("original_performance") is not None:
            filtered_ids.append(bench_id)

    # Write updated vector.json
    vector["benchmarks"] = filtered_ids
    with vector_path.open("w", encoding="utf-8") as f:
        json.dump(vector, f, indent=2)

    # Build performance subset
    subset = {k: v for k, v in perf.items() if k != "benchmarks"}
    subset["benchmarks"] = [
        id_to_entry[b] for b in filtered_ids if b in id_to_entry
    ]
    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w", encoding="utf-8") as f:
        json.dump(subset, f, indent=2)

    print(f"Updated {vector_path} with {len(filtered_ids)} benchmarks")
    print(f"Wrote {output_path}")


if __name__ == "__main__":
    main()
