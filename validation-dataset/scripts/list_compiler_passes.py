#!/usr/bin/env python3
"""List unique compiler optimization passes from report JSON files.

The script scans a directory of benchmark JSON reports and aggregates all
compiler optimization pass names. Earlier versions only considered the
``compiler_optimizations`` section.  Some reports also record detailed GCC pass
information under ``gcc_passes`` which should be included as well.  This script
now accounts for both sources.
"""

import argparse
import json
from pathlib import Path


def collect_passes(report_dir: Path) -> set[str]:
    """Collect all unique compiler optimization pass names.

    Parameters
    ----------
    report_dir : Path
        Directory containing per-benchmark JSON report files.

    Returns
    -------
    set[str]
        The union of every compiler pass name referenced either in the
        ``compiler_optimizations`` or ``gcc_passes`` sections of the reports.
    """

    passes: set[str] = set()
    for path in report_dir.glob("benchmark_*.json"):
        try:
            with open(path, "r", encoding="utf-8") as f:
                data = json.load(f)
        except Exception:
            continue

        comp_opts = data.get("compiler_optimizations")
        if isinstance(comp_opts, dict):
            passes.update(comp_opts.keys())

        gcc_passes = data.get("gcc_passes")
        if isinstance(gcc_passes, dict):
            for build in gcc_passes.values():
                if isinstance(build, dict):
                    passes.update(build.keys())

    return passes


def main() -> None:
    parser = argparse.ArgumentParser(description="List compiler optimization passes")
    parser.add_argument(
        "--report-dir",
        default=Path("reports/gpt-4o-mini"),
        type=Path,
        help="Directory containing benchmark JSON reports",
    )
    parser.add_argument(
        "--output", default="optimization_passes.txt", help="Output text file"
    )
    args = parser.parse_args()

    all_passes = sorted(collect_passes(args.report_dir))
    with open(args.output, "w", encoding="utf-8") as f:
        for p in all_passes:
            f.write(f"{p}\n")
    print(f"Found {len(all_passes)} passes. Wrote {args.output}")


if __name__ == "__main__":
    main()
