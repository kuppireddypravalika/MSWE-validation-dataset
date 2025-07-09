#!/usr/bin/env python3
"""Aggregate optimization suggestions from code comparison reports."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def collect_optimizations(report_dir: Path) -> set[str]:
    """Return a set of optimization strings from JSON reports."""
    optimizations: set[str] = set()
    for path in report_dir.glob("benchmark_*_*.json"):
        with path.open(encoding="utf-8") as f:
            data = json.load(f)
        for field in ("missing_optimizations", "additional_optimizations"):
            values = data.get(field, [])
            if isinstance(values, list):
                for item in values:
                    if isinstance(item, str):
                        optimizations.add(item.strip())
    return optimizations


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Collect optimizations from code comparison reports"
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark reports",
    )
    parser.add_argument(
        "--output",
        default="reports/optimization_knowledge_base.json",
        help="File to write aggregated optimizations",
    )
    args = parser.parse_args()

    report_dir = Path(args.reports_dir)
    optimizations = sorted(collect_optimizations(report_dir))

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        json.dump({"optimizations": optimizations}, f, indent=2)


if __name__ == "__main__":
    main()
