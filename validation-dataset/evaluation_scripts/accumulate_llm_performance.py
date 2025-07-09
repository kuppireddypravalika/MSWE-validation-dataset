#!/usr/bin/env python3
"""Aggregate LLM speedup metrics by inefficiency category."""

import argparse
import csv
import json
import math
from collections import defaultdict
from pathlib import Path


def load_mapping(json_path: Path) -> dict:
    """Return benchmark -> inefficiency mapping."""
    with json_path.open() as f:
        data = json.load(f)
    return {b["name"]: b.get("inefficiency", "") for b in data.get("benchmarks", [])}


def aggregate_speedups(csv_path: Path, mapping: dict) -> list[dict]:
    """Return aggregated speedups grouped by inefficiency."""
    buckets: defaultdict[str, list[float]] = defaultdict(list)
    with csv_path.open(newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            bench = row.get("benchmark")
            ineff = mapping.get(bench)
            if not ineff:
                continue
            try:
                speedup = float(row.get("speedup_llm_vs_baseline", "nan"))
            except ValueError:
                continue
            if math.isnan(speedup):
                continue
            buckets[ineff].append(speedup)
    results = []
    for ineff, vals in sorted(buckets.items()):
        avg = sum(vals) / len(vals) if vals else float("nan")
        results.append(
            {
                "inefficiency": ineff,
                "benchmarks": len(vals),
                "avg_speedup_llm_vs_baseline": avg,
            }
        )
    return results


def write_csv(rows: list[dict], out_path: Path) -> None:
    """Write aggregated rows to a CSV file."""
    fieldnames = ["inefficiency", "benchmarks", "avg_speedup_llm_vs_baseline"]
    with out_path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)
    print(f"Summary written to {out_path}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Summarize LLM performance by inefficiency category"
    )
    parser.add_argument(
        "--benchmarks-json",
        default="existing_benchmarks.json",
        help="Path to existing_benchmarks.json",
    )
    parser.add_argument(
        "--report",
        default="reports/accumulated_report.csv",
        help="CSV with per-benchmark performance metrics",
    )
    parser.add_argument(
        "--output",
        default="reports/llm_perf_by_inefficiency.csv",
        help="Output CSV file",
    )
    args = parser.parse_args()

    mapping = load_mapping(Path(args.benchmarks_json))
    rows = aggregate_speedups(Path(args.report), mapping)

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    write_csv(rows, out_path)


if __name__ == "__main__":
    main()
