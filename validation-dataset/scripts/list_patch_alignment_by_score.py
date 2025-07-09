#!/usr/bin/env python3
"""List benchmarks with a patch alignment score for a given dimension."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

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


def collect_matches(report_dir: Path, dimension: str, score: int) -> list[str]:
    """Return benchmarks whose selected dimension has the given score."""
    matches: list[str] = []
    for path in report_dir.glob("benchmark_*.json"):
        try:
            with path.open(encoding="utf-8") as f:
                data = json.load(f)
        except Exception:
            continue
        dim_data = data.get("scores", {}).get(dimension)
        if not isinstance(dim_data, dict):
            continue
        val = dim_data.get("score")
        if isinstance(val, (int, float)) and int(val) == score:
            matches.append(path.stem)
    return matches


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List benchmarks matching a patch alignment score",
    )
    parser.add_argument(
        "--dimension",
        required=True,
        choices=DIMENSIONS,
        help="Patch alignment dimension to match",
    )
    parser.add_argument(
        "--score",
        type=int,
        required=True,
        help="Score to match",
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/patch_alignment",
        help="Directory with patch alignment JSON files",
    )
    args = parser.parse_args()

    matches = sorted(collect_matches(Path(args.reports_dir), args.dimension, args.score))
    for name in matches:
        print(name)


if __name__ == "__main__":
    main()
