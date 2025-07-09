#!/usr/bin/env python3
"""List benchmarks containing optimizations for a given category."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def load_mapping(path: Path) -> dict[str, dict[str, str]]:
    """Return mapping of optimization string to category info."""
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def matches(entry: dict[str, str], category: str, subcategory: str | None) -> bool:
    """Return True if mapping entry matches category and optional subcategory."""
    if entry.get("category") != category:
        return False
    if subcategory and entry.get("subcategory") != subcategory:
        return False
    return True


def passes_filter(perf: dict, *, compare_to: str,
                  slower_than: float | None,
                  faster_than: float | None) -> bool:
    """Return True if performance metrics satisfy filtering criteria."""
    llm = perf.get("llm_over_original")
    if llm is None or not isinstance(llm, (int, float)) or llm <= 0:
        return False

    if compare_to == "baseline":
        base = perf.get("baseline_over_original")
        if base is None or not isinstance(base, (int, float)) or base <= 0:
            return False
        ratio = base / llm  # LLM time / baseline time
    else:
        ratio = 1 / llm  # LLM time / original time

    if slower_than is not None and ratio < 1 + slower_than:
        return False
    if faster_than is not None and ratio > 1 - faster_than:
        return False
    return True


def collect_benchmarks(
    report_dir: Path,
    mapping: dict[str, dict[str, str]],
    *,
    category: str,
    subcategory: str | None,
    field: str,
    compare_to: str,
    slower_than: float | None,
    faster_than: float | None,
) -> set[str]:
    """Return benchmark names containing optimizations for the given category."""
    results: set[str] = set()
    for path in report_dir.glob("benchmark_*.json"):
        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        perf = data.get("performance", {})
        if not passes_filter(perf, compare_to=compare_to,
                             slower_than=slower_than,
                             faster_than=faster_than):
            continue

        optimizations = data.get(field, [])
        if not isinstance(optimizations, list):
            continue
        for opt in optimizations:
            if not isinstance(opt, str):
                continue
            info = mapping.get(opt.strip())
            if info and matches(info, category, subcategory):
                results.add(path.stem)
                break
    return results


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List benchmark directories by optimization category",
    )
    parser.add_argument(
        "--category",
        required=True,
        help="Optimization category to match",
    )
    parser.add_argument(
        "--subcategory",
        help="Optional subcategory to match",
    )
    parser.add_argument(
        "--flag",
        choices=["missed", "addition"],
        help=(
            "Consider missing or additional optimizations. "
            "If not provided, both are included"
        ),
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark comparison JSON files",
    )
    parser.add_argument(
        "--mapping",
        default="reports/optimization_category_mapping.json",
        help="JSON file mapping optimizations to categories",
    )
    parser.add_argument(
        "--compare-to",
        choices=["baseline", "original"],
        default="baseline",
        help="Reference implementation for performance filters",
    )
    parser.add_argument(
        "--slower-than",
        type=float,
        help="Include benchmarks where LLM is this fraction or more slower",
    )
    parser.add_argument(
        "--faster-than",
        type=float,
        help="Include benchmarks where LLM is this fraction or more faster",
    )
    args = parser.parse_args()

    mapping = load_mapping(Path(args.mapping))
    report_dir = Path(args.reports_dir)

    if args.flag:
        field = (
            "missing_optimizations"
            if args.flag == "missed"
            else "additional_optimizations"
        )
        benchmarks = collect_benchmarks(
            report_dir,
            mapping,
            category=args.category,
            subcategory=args.subcategory,
            field=field,
            compare_to=args.compare_to,
            slower_than=args.slower_than,
            faster_than=args.faster_than,
        )
    else:
        missed = collect_benchmarks(
            report_dir,
            mapping,
            category=args.category,
            subcategory=args.subcategory,
            field="missing_optimizations",
            compare_to=args.compare_to,
            slower_than=args.slower_than,
            faster_than=args.faster_than,
        )
        addition = collect_benchmarks(
            report_dir,
            mapping,
            category=args.category,
            subcategory=args.subcategory,
            field="additional_optimizations",
            compare_to=args.compare_to,
            slower_than=args.slower_than,
            faster_than=args.faster_than,
        )
        benchmarks = missed | addition

    for name in sorted(benchmarks):
        print(name)


if __name__ == "__main__":
    main()
