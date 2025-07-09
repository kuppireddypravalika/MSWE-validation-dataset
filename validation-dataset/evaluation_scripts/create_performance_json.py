#!/usr/bin/env python3
"""Generate JSON summarizing benchmark speedups."""

import argparse
import csv
import json
from pathlib import Path


def clean(val: float) -> float | None:
    """Return None for NaN, otherwise the value."""
    return None if val != val else val


def load_benchmarks(json_path: Path) -> list[dict]:
    """Return list of benchmarks with inefficiency and optimization."""
    with json_path.open() as f:
        data = json.load(f)
    return data.get("benchmarks", [])


def load_speedups(csv_path: Path) -> dict[str, dict[str, float]]:
    """Return speedup metrics keyed by benchmark name."""
    result: dict[str, dict[str, float]] = {}
    with csv_path.open(newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            name = row.get("benchmark")
            if not name:
                continue
            try:
                llm = float(row.get("speedup_llm_vs_orig", "nan"))
            except ValueError:
                llm = float("nan")
            try:
                base = float(row.get("speedup_baseline_vs_orig", "nan"))
            except ValueError:
                base = float("nan")
            result[name] = {
                "llm_over_original": clean(llm),
                "baseline_over_original": clean(base),
            }
    return result


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create a benchmark performance summary JSON"
    )
    parser.add_argument(
        "--benchmarks-json",
        default="existing_benchmarks.json",
        help="Input JSON mapping benchmarks to categories",
    )
    parser.add_argument(
        "--report",
        default="reports/accumulated_report.csv",
        help="CSV with per-benchmark performance metrics",
    )
    parser.add_argument(
        "--output",
        default="reports/benchmark_performance.json",
        help="Output JSON file",
    )
    args = parser.parse_args()

    benches = load_benchmarks(Path(args.benchmarks_json))
    speedups = load_speedups(Path(args.report))

    results = []
    for bench in benches:
        name = bench.get("name")
        perf = speedups.get(name, {})
        results.append(
            {
                "name": name,
                "inefficiency": bench.get("inefficiency"),
                "optimization": bench.get("optimization"),
                "llm_over_original": perf.get("llm_over_original"),
                "baseline_over_original": perf.get("baseline_over_original"),
            }
        )

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        json.dump({"benchmarks": results}, f, indent=2)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
