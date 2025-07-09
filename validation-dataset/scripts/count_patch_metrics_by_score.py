#!/usr/bin/env python3
"""List benchmarks with a given patch metric score and print the total count.

This script supports selecting multiple dimensions in a single
invocation.  You may supply ``--dimension``/``--score`` pairs
repeatedly or provide one score for several dimensions.  ``--score``
may optionally start with ``>``, ``>=``, ``<`` or ``<=`` to select a
range of scores rather than a single value.  By default each
dimension's matches and total count are printed separately in the
order provided on the command line.  Use ``--intersection`` to print
only benchmarks that satisfy **all** supplied conditions.
"""

from __future__ import annotations

import argparse
import json
from dataclasses import dataclass
from pathlib import Path


@dataclass
class ScoreCondition:
    op: str
    value: int

    def matches(self, val: int) -> bool:
        if self.op == '>':
            return val > self.value
        if self.op == '>=':
            return val >= self.value
        if self.op == '<':
            return val < self.value
        if self.op == '<=':
            return val <= self.value
        return val == self.value


def parse_score_condition(text: str) -> ScoreCondition:
    for prefix in ('>=', '<=', '>', '<'):
        if text.startswith(prefix):
            try:
                num = int(text[len(prefix):])
            except ValueError as exc:
                raise argparse.ArgumentTypeError(f'Invalid score: {text!r}') from exc
            return ScoreCondition(prefix, num)
    try:
        num = int(text)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f'Invalid score: {text!r}') from exc
    return ScoreCondition('==', num)

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


def collect_matches(
    report_dir: Path, dimension: str, condition: ScoreCondition
) -> list[str]:
    """Return benchmarks whose selected dimension satisfies the score condition."""
    matches: list[str] = []
    for path in report_dir.glob("benchmark_*.json"):
        try:
            with path.open(encoding="utf-8") as f:
                data = json.load(f)
        except Exception:
            continue

        # Older reports store metrics under a top-level "scores" key while
        # newer ones keep the metrics directly at the root.  Support both
        # formats by picking whichever dictionary contains the dimension.
        scores_dict = data.get("scores")
        if isinstance(scores_dict, dict) and dimension in scores_dict:
            dim_data = scores_dict.get(dimension)
        else:
            dim_data = data.get(dimension)

        if not isinstance(dim_data, dict):
            continue
        val = dim_data.get("score")
        if isinstance(val, (int, float)) and condition.matches(int(val)):
            matches.append(path.stem)
    return matches


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List benchmarks matching a patch metric score and count them",
    )
    parser.add_argument(
        "--dimension",
        dest="dimensions",
        action="append",
        required=True,
        choices=DIMENSIONS,
        help="Patch metric dimension to match (repeatable)",
    )
    parser.add_argument(
        "--score",
        dest="scores",
        action="append",
        type=parse_score_condition,
        required=True,
        help=(
            "Score for the preceding --dimension (repeatable). "
            "Prefix with >, >=, < or <= to compare"
        ),
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/patch_alignment",
        help="Directory with patch metric JSON files",
    )
    parser.add_argument(
        "--intersection",
        action="store_true",
        help="Print only benchmarks that satisfy all conditions",
    )
    args = parser.parse_args()

    report_dir = Path(args.reports_dir)
    # If only one score is provided but multiple dimensions, reuse the score
    if len(args.scores) == 1 and len(args.dimensions) > 1:
        args.scores = args.scores * len(args.dimensions)

    if len(args.dimensions) != len(args.scores):
        parser.error("Must supply the same number of --dimension and --score options")

    all_matches = []
    for dim, score in zip(args.dimensions, args.scores):
        all_matches.append(set(collect_matches(report_dir, dim, score)))

    if args.intersection:
        if not all_matches:
            matches: list[str] = []
        else:
            matches = sorted(set.intersection(*all_matches))
        for name in matches:
            print(name)
        print(f"Total: {len(matches)}")
    else:
        for dim, matches_set in zip(args.dimensions, all_matches):
            matches = sorted(matches_set)
            if len(args.dimensions) > 1:
                print(f"=== {dim} ===")
            for name in matches:
                print(name)
            print(f"Total: {len(matches)}")
            if len(args.dimensions) > 1:
                print()


if __name__ == "__main__":
    main()
