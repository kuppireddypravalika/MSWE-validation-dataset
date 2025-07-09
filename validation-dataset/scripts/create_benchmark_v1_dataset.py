#!/usr/bin/env python3
"""Copy patch files from existing benchmarks to benchmark_v1 directories."""

from __future__ import annotations

import argparse
import json
import shutil
from pathlib import Path


def copy_patch_files(src: Path, dest: Path) -> None:
    """Copy *.before, *.after and *.patch files from src to dest."""
    dest.mkdir(parents=True, exist_ok=True)
    for pattern in ("*.before", "*.after", "*.patch"):
        for path in src.glob(pattern):
            shutil.copy(path, dest / path.name)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Create benchmark_v1 dataset from existing benchmarks"
    )
    parser.add_argument(
        "--benchmarks-json",
        default="existing_benchmarks.json",
        help="JSON file listing benchmarks to copy",
    )
    parser.add_argument(
        "--benchmarks-dir", default="benchmarks", help="Directory containing benchmarks"
    )
    parser.add_argument(
        "--output-json",
        default="existing_benchmarks_v1.json",
        help="Output JSON listing new benchmarks",
    )
    args = parser.parse_args()

    with open(args.benchmarks_json, encoding="utf-8") as f:
        data = json.load(f)

    src_root = Path(args.benchmarks_dir)
    dest_root = src_root

    new_entries = []
    for idx, entry in enumerate(data.get("benchmarks", []), start=1):
        if isinstance(entry, dict):
            name = entry.get("name", "")
        else:
            name = str(entry)
            entry = {"name": name}

        new_name = f"benchmark_v1_{idx:03d}"
        copy_patch_files(src_root / name, dest_root / new_name)

        new_entry = dict(entry)
        new_entry["name"] = new_name
        new_entries.append(new_entry)

    with open(args.output_json, "w", encoding="utf-8") as f:
        json.dump({"benchmarks": new_entries}, f, indent=2)


if __name__ == "__main__":
    main()
