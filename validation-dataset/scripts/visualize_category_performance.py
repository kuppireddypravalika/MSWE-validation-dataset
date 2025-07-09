#!/usr/bin/env python3
"""Generate performance plots for each category in category_packages."""
from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Iterable

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns


def sanitize(name: str) -> str:
    """Return a filesystem-friendly name."""
    return "".join(c if c.isalnum() or c in {"-", "_"} else "_" for c in name)


def load_times(perf_file: Path) -> tuple[list[float], list[float], list[float]]:
    """Return lists of original, human, and best LLM runtimes for a performance JSON."""
    with perf_file.open() as f:
        data = json.load(f)
    orig_times: list[float] = []
    human_times: list[float] = []
    llm_times: list[float] = []
    for bench in data.get("benchmarks", []):
        orig_entry = bench.get("original_performance") or {}
        human_entry = bench.get("human_performance") or {}
        orig = orig_entry.get("mean")
        human = human_entry.get("mean")
        llm_solutions = bench.get("llm_solutions", [])
        if orig is None or human is None:
            continue
        best_llm = None
        for sol in llm_solutions:
            if not sol.get("runnable"):
                continue
            perf = sol.get("performance", {})
            mean = perf.get("mean")
            if mean is None:
                continue
            if best_llm is None or mean < best_llm:
                best_llm = mean
        if best_llm is None:
            continue
        orig_times.append(orig)
        human_times.append(human)
        llm_times.append(best_llm)
    return orig_times, human_times, llm_times


def bar_plot(means: Iterable[float], labels: list[str], title: str, path: Path) -> None:
    plt.figure()
    sns.barplot(x=labels, y=list(means))
    plt.ylabel("Mean Runtime (ms)")
    plt.title(title)
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def pdf_plot(ratios: np.ndarray, title: str, path: Path) -> None:
    plt.figure()
    sns.histplot(ratios, kde=True, stat="density")
    plt.xlabel("Improvement Factor (original / LLM)")
    plt.title(title)
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def cdf_plot(ratios: np.ndarray, title: str, path: Path) -> None:
    sorted_ratios = np.sort(ratios)
    cdf = np.arange(1, len(sorted_ratios) + 1) / len(sorted_ratios)
    plt.figure()
    plt.plot(sorted_ratios, cdf, marker=".")
    plt.xlabel("Improvement Factor (original / LLM)")
    plt.ylabel("CDF")
    plt.grid(True)
    plt.title(title)
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def scatter_plot(orig: Iterable[float], llm: Iterable[float], title: str, path: Path) -> None:
    plt.figure()
    plt.scatter(orig, llm)
    plt.xlabel("Original Runtime (ms)")
    plt.ylabel("Best LLM Runtime (ms)")
    plt.grid(True)
    plt.title(title)
    plt.tight_layout()
    plt.savefig(path, format="svg")
    plt.close()


def analyze_category(cat_dir: Path, out_dir: Path) -> None:
    perf_file = cat_dir / "benchmark_performance.json"
    if not perf_file.is_file():
        return
    orig_times, human_times, llm_times = load_times(perf_file)
    if not orig_times:
        return
    cat_name = cat_dir.name
    safe = sanitize(cat_name)
    out_dir.mkdir(parents=True, exist_ok=True)
    arr_orig = np.array(orig_times, dtype=float)
    arr_llm = np.array(llm_times, dtype=float)
    mask = arr_llm > 0
    ratios = arr_orig[mask] / arr_llm[mask]
    bar_plot(
        [np.mean(orig_times), np.mean(human_times), np.mean(llm_times)],
        ["Original", "Developer", "LLM"],
        f"{cat_name} - Mean Runtime",
        out_dir / f"{safe}_bar.svg",
    )
    pdf_plot(ratios, f"{cat_name} - Improvement PDF", out_dir / f"{safe}_pdf.svg")
    cdf_plot(ratios, f"{cat_name} - Improvement CDF", out_dir / f"{safe}_cdf.svg")
    scatter_plot(
        orig_times,
        llm_times,
        f"{cat_name} - Original vs LLM",
        out_dir / f"{safe}_scatter.svg",
    )


def create_markdown(categories: list[str], out_dir: Path) -> None:
    md_path = out_dir / "category_analysis.md"
    with md_path.open("w", encoding="utf-8") as f:
        f.write("# Category Performance Analysis\n\n")
        for cat_name in categories:
            safe = sanitize(cat_name)
            f.write(f"## {cat_name}\n\n")
            f.write("| | |\n")
            f.write("|---|---|\n")
            f.write(
                f"| ![{safe} bar](./{safe}_bar.svg) | ![{safe} pdf](./{safe}_pdf.svg) |\n"
            )
            f.write(
                f"| ![{safe} cdf](./{safe}_cdf.svg) | ![{safe} scatter](./{safe}_scatter.svg) |\n\n"
            )


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate performance visualizations for each category",
    )
    parser.add_argument(
        "--categories-dir",
        default="category_packages",
        help="Directory containing category subfolders",
    )
    parser.add_argument(
        "--output-dir",
        default="reports/category_analysis",
        help="Directory to store generated plots and markdown",
    )
    args = parser.parse_args()

    categories_root = Path(args.categories_dir)
    out_dir = Path(args.output_dir)
    categories = []
    for cat_dir in sorted(categories_root.iterdir()):
        if not cat_dir.is_dir():
            continue
        perf_file = cat_dir / "benchmark_performance.json"
        if not perf_file.is_file():
            continue
        categories.append(cat_dir.name)
        analyze_category(cat_dir, out_dir)
    if categories:
        create_markdown(categories, out_dir)
        print(f"Wrote {out_dir / 'category_analysis.md'}")


if __name__ == "__main__":
    main()
