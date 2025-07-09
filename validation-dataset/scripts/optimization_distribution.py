#!/usr/bin/env python3
"""Create a heatmap visualization of optimization category distribution from comparison reports."""

from __future__ import annotations

import argparse
import json
from collections import Counter
from pathlib import Path

import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd


def sanitize(name: str) -> str:
    """Return a filesystem-friendly version of ``name``."""
    return "".join(c if c.isalnum() or c in {"-", "_"} else "_" for c in name)


def load_mapping(path: Path) -> dict[str, str]:
    with path.open(encoding="utf-8") as f:
        data = json.load(f)
    mapping: dict[str, str] = {}
    for key, val in data.items():
        if isinstance(val, dict):
            cat = val.get("subcategory")
            if isinstance(cat, str):
                mapping[key.strip()] = cat
    return mapping


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
    else:  # compare to original
        ratio = 1 / llm  # LLM time / original time

    if slower_than is not None and ratio < 1 + slower_than:
        return False
    if faster_than is not None and ratio > 1 - faster_than:
        return False
    return True


def collect_counts(
    report_dir: Path,
    mapping: dict[str, str],
    *,
    compare_to: str,
    slower_than: float | None,
    faster_than: float | None,
) -> tuple[Counter, Counter]:
    missing_counts: Counter[str] = Counter()
    additional_counts: Counter[str] = Counter()

    for path in report_dir.glob("benchmark*_*.json"):
        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        perf = data.get("performance", {})
        if not passes_filter(perf, compare_to=compare_to,
                             slower_than=slower_than, faster_than=faster_than):
            continue

        for item in data.get("missing_optimizations", []):
            if not isinstance(item, str):
                continue
            cat = mapping.get(item.strip())
            if cat:
                missing_counts[cat] += 1
        for item in data.get("additional_optimizations", []):
            if not isinstance(item, str):
                continue
            cat = mapping.get(item.strip())
            if cat:
                additional_counts[cat] += 1

    return missing_counts, additional_counts


def plot_heatmap(missing: Counter, additional: Counter, path: Path) -> None:
    all_categories = sorted(set(missing) | set(additional))

    df = pd.DataFrame({
        "Additional": [additional.get(cat, 0) for cat in all_categories],
        "Missing": [missing.get(cat, 0) for cat in all_categories],
    }, index=all_categories)

    plt.figure(figsize=(10, 8))
    sns.heatmap(df, annot=True, fmt="d", cmap="Blues")
    plt.title("Optimization Categories: Missing vs Additional")
    plt.ylabel("Optimization Categories")
    plt.xlabel("Optimization Type")
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Plot heatmap of optimization category distribution",
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark comparison JSON files",
    )
    parser.add_argument(
        "--mapping-file",
        default="reports/optimization_category_mapping.json",
        help="JSON file mapping optimization text to categories",
    )
    parser.add_argument(
        "--output-dir",
        default="reports/optimization_distribution",
        help="Directory to save plots",
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

    report_dir = Path(args.reports_dir)
    mapping_path = Path(args.mapping_file)
    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    mapping = load_mapping(mapping_path)
    missing, additional = collect_counts(
        report_dir,
        mapping,
        compare_to=args.compare_to,
        slower_than=args.slower_than,
        faster_than=args.faster_than,
    )

    name_parts = ["optimization_sub_heatmap", args.compare_to]
    if args.slower_than is not None:
        name_parts.append(f"slow_{args.slower_than}")
    if args.faster_than is not None:
        name_parts.append(f"fast_{args.faster_than}")
    file_name = "_".join(sanitize(str(p)) for p in name_parts) + ".svg"
    out_path = out_dir / file_name

    plot_heatmap(missing, additional, out_path)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
