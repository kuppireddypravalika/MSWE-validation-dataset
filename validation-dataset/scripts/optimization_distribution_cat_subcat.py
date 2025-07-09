#!/usr/bin/env python3
"""Create heatmaps for optimization category and sub-category distribution from comparison reports, grouping sub-categories by categories."""

from __future__ import annotations

import argparse
import json
from collections import Counter, defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd


def load_mapping(path: Path) -> dict[str, tuple[str, str]]:
    with path.open(encoding="utf-8") as f:
        data = json.load(f)
    mapping: dict[str, tuple[str, str]] = {}
    for key, val in data.items():
        if isinstance(val, dict):
            category = val.get("category")
            subcategory = val.get("subcategory")
            if isinstance(category, str) and isinstance(subcategory, str):
                mapping[key.strip()] = (category, subcategory)
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
    else:
        ratio = 1 / llm  # LLM time / original time

    if slower_than is not None and ratio < 1 + slower_than:
        return False
    if faster_than is not None and ratio > 1 - faster_than:
        return False
    return True


def collect_counts(
    report_dir: Path,
    mapping: dict[str, tuple[str, str]],
    *,
    compare_to: str,
    slower_than: float | None,
    faster_than: float | None,
) -> tuple[Counter, Counter, Counter, Counter]:
    missing_counts_cat: Counter[str] = Counter()
    additional_counts_cat: Counter[str] = Counter()
    missing_counts_subcat: Counter[str] = Counter()
    additional_counts_subcat: Counter[str] = Counter()

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
            cat_subcat = mapping.get(item.strip())
            if cat_subcat:
                cat, subcat = cat_subcat
                missing_counts_cat[cat] += 1
                missing_counts_subcat[(cat, subcat)] += 1
        for item in data.get("additional_optimizations", []):
            if not isinstance(item, str):
                continue
            cat_subcat = mapping.get(item.strip())
            if cat_subcat:
                cat, subcat = cat_subcat
                additional_counts_cat[cat] += 1
                additional_counts_subcat[(cat, subcat)] += 1

    return missing_counts_cat, additional_counts_cat, missing_counts_subcat, additional_counts_subcat


def collect_benchmarks_for_category(
    report_dir: Path,
    mapping: dict[str, tuple[str, str]],
    *,
    category: str,
    compare_to: str,
    slower_than: float | None,
    faster_than: float | None,
) -> set[str]:
    """Return benchmark names containing optimizations for the given category."""
    results: set[str] = set()
    for path in report_dir.glob("benchmark*_*.json"):
        with path.open(encoding="utf-8") as f:
            data = json.load(f)

        perf = data.get("performance", {})
        if not passes_filter(perf, compare_to=compare_to,
                             slower_than=slower_than, faster_than=faster_than):
            continue

        found = False
        for item in data.get("missing_optimizations", []):
            if not isinstance(item, str):
                continue
            cat_subcat = mapping.get(item.strip())
            if cat_subcat and cat_subcat[0] == category:
                results.add(path.stem)
                found = True
                break
        if found:
            continue
        for item in data.get("additional_optimizations", []):
            if not isinstance(item, str):
                continue
            cat_subcat = mapping.get(item.strip())
            if cat_subcat and cat_subcat[0] == category:
                results.add(path.stem)
                break
    return results


def plot_heatmap(missing: Counter, additional: Counter, title: str, path: Path, group_by_category: bool = False) -> None:
    if group_by_category:
        categories = defaultdict(list)
        for (cat, subcat) in set(missing) | set(additional):
            categories[cat].append(subcat)

        ordered_items = []
        for cat in sorted(categories):
            ordered_items.extend([(cat, subcat) for subcat in sorted(categories[cat])])

        labels = [f"{subcat} ({cat})" for cat, subcat in ordered_items]
        df = pd.DataFrame({
            "Additional": [additional.get(item, 0) for item in ordered_items],
            "Missing": [missing.get(item, 0) for item in ordered_items],
        }, index=labels)
    else:
        all_items = sorted(set(missing) | set(additional))
        df = pd.DataFrame({
            "Additional": [additional.get(item, 0) for item in all_items],
            "Missing": [missing.get(item, 0) for item in all_items],
        }, index=all_items)

    plt.figure(figsize=(12, max(6, len(df) * 0.5)))
    sns.heatmap(df, annot=True, fmt="d", cmap="Blues")
    plt.title(title)
    plt.ylabel("Categories/Sub-categories")
    plt.xlabel("Optimization Type")
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Plot heatmaps of optimization category and sub-category distribution grouped by categories",
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark comparison JSON files",
    )
    parser.add_argument(
        "--mapping-file",
        default="reports/optimization_category_mapping.json",
        help="JSON file mapping optimization text to categories and sub-categories",
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
    missing_cat, additional_cat, missing_subcat, additional_subcat = collect_counts(
        report_dir,
        mapping,
        compare_to=args.compare_to,
        slower_than=args.slower_than,
        faster_than=args.faster_than,
    )

    plot_heatmap(missing_cat, additional_cat, "Optimization Categories: Missing vs Additional", out_dir / "optimization_categories_heatmap.svg")
    plot_heatmap(missing_subcat, additional_subcat, "Optimization Sub-categories: Missing vs Additional", out_dir / "optimization_subcategories_heatmap.svg", group_by_category=True)

    mm_benchmarks = collect_benchmarks_for_category(
        report_dir,
        mapping,
        category="Memory Management Optimizations",
        compare_to=args.compare_to,
        slower_than=args.slower_than,
        faster_than=args.faster_than,
    )
    if mm_benchmarks:
        print("Benchmarks with Memory Management Optimizations:")
        for name in sorted(mm_benchmarks):
            print(name)
    else:
        print("No benchmarks with Memory Management Optimizations found.")


if __name__ == "__main__":
    main()
