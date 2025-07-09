#!/usr/bin/env python3
"""Generate per-category analysis_results JSON files."""

from __future__ import annotations

import argparse
import subprocess
from pathlib import Path

from evaluation_scripts.analyze_performance import _sanitize


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Run analysis on each category's performance JSON"
    )
    parser.add_argument(
        "--categories-dir",
        default="category_packages",
        help="Directory containing category subfolders",
    )
    parser.add_argument(
        "--analysis-script",
        default="evaluation_scripts/analyze_performance.py",
        help="Path to analyze_performance.py",
    )
    parser.add_argument("--model", default="gpt-4o-mini", help="Model name")
    parser.add_argument("--variant", default="single-step", help="Model variant")
    args = parser.parse_args()

    categories_root = Path(args.categories_dir)
    for cat_dir in categories_root.iterdir():
        if not cat_dir.is_dir():
            continue
        perf_file = cat_dir / "benchmark_performance.json"
        if not perf_file.is_file():
            continue
        cat_name = cat_dir.name
        out_file = cat_dir / (
            f"analysis_results_{_sanitize(args.model)}_"
            f"{_sanitize(args.variant)}_{_sanitize(cat_name)}.json"
        )
        cmd = [
            "python",
            args.analysis_script,
            "--input",
            str(perf_file),
            "--variant",
            args.variant,
            "--output",
            str(out_file),
        ]
        subprocess.run(cmd, check=True)


if __name__ == "__main__":
    main()
