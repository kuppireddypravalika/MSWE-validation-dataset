#!/usr/bin/env python3
"""Plot score distributions for patch alignment reports."""

from __future__ import annotations

import argparse
import json
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
import seaborn as sns


DIMENSIONS = [
    "structural_fidelity",
    "targeted_isolation",
    "side_effect_preservation",
    "correctness_and_equivalence",
    "computational_realism",
    "context_neutrality",
    "performance_optimization_relevance",
    "contextual_dependency",
    "library_api_dependency",
]


def collect_scores(report_dir: Path) -> dict[str, list[int]]:
    """Return mapping from dimension to list of scores."""
    scores: dict[str, list[int]] = defaultdict(list)
    for path in report_dir.glob("benchmark_*.json"):
        try:
            with path.open(encoding="utf-8") as f:
                data = json.load(f)
        except Exception:
            continue
        dim_data = data.get("scores")
        if not isinstance(dim_data, dict):
            continue
        for dim in DIMENSIONS:
            entry = dim_data.get(dim)
            if isinstance(entry, dict):
                val = entry.get("score")
                if isinstance(val, (int, float)):
                    scores[dim].append(int(val))
    return scores


def plot_distributions(scores: dict[str, list[int]], out_path: Path) -> None:
    """Create a histogram for each dimension."""
    n_dims = len(DIMENSIONS)
    cols = 3
    rows = (n_dims + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(4 * cols, 4 * rows))
    axes_list = axes.flatten()
    for idx, dim in enumerate(DIMENSIONS):
        ax = axes_list[idx]
        vals = scores.get(dim, [])
        if vals:
            sns.histplot(vals, bins=[0.5, 1.5, 2.5, 3.5, 4.5, 5.5], discrete=True, ax=ax)
        ax.set_title(dim.replace("_", " ").title())
        ax.set_xlabel("Score")
        ax.set_ylabel("Count")
        ax.set_xticks(range(1, 6))
    for ax in axes_list[n_dims:]:
        ax.axis("off")
    plt.tight_layout()
    out_path.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(out_path, format="svg")
    plt.close()


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create score distribution plots for patch alignment reports",
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/patch_alignment",
        help="Directory containing patch alignment JSON files",
    )
    parser.add_argument(
        "--output",
        default="reports/patch_alignment/score_distribution.svg",
        help="Path to save generated plot",
    )
    args = parser.parse_args()

    report_dir = Path(args.reports_dir)
    out_path = Path(args.output)

    scores = collect_scores(report_dir)
    plot_distributions(scores, out_path)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
