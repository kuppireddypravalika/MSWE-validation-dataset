#!/usr/bin/env python3
"""Utility to extract GCC optimization pass information from dump files."""

import os
import re
from collections import defaultdict
from typing import Dict, List, Optional


def extract_gcc_passes(dump_dir: str, source_basename: Optional[str] = None) -> Dict[str, Dict[str, List[int]]]:
    """Scan a directory of GCC dump files and aggregate optimization passes.

    Parameters
    ----------
    dump_dir : str
        Directory containing GCC dump files generated with ``-fdump-*-all-all``.
    source_basename : Optional[str], default None
        If provided, only consider dump files starting with this basename. The
        basename should include the source file's extension (e.g. ``source.c``).

    Returns
    -------
    Dict[str, Dict[str, List[int]]]
        Mapping from optimization pass name to a dictionary with ``count`` and
        ``positions`` keys. ``positions`` lists the numeric prefixes extracted
        from the filenames in ascending order.
    """

    passes: Dict[str, Dict[str, List[int]]] = defaultdict(lambda: {"count": 0, "positions": []})

    if not os.path.isdir(dump_dir):
        raise FileNotFoundError(f"Directory not found: {dump_dir}")

    # GCC uses names like `ipa-clones` or `type-inheritance` which include
    # hyphens in the dump filenames. Allow dashes in the optimization pass
    # group to avoid silently skipping those entries.
    pattern = re.compile(
        r"^(?P<base>.+?)\.(?P<num>\d+)[a-z]+\.(?P<pass>[A-Za-z0-9_-]+)$"
    )

    for fname in os.listdir(dump_dir):
        match = pattern.match(fname)
        if not match:
            continue

        base_name = match.group("base")
        if source_basename and not base_name.endswith(source_basename):
            continue

        pass_name = match.group("pass")
        position = int(match.group("num"))
        entry = passes[pass_name]
        entry["count"] += 1
        entry["positions"].append(position)

    # Sort positions for consistency
    for entry in passes.values():
        entry["positions"].sort()

    return dict(passes)


if __name__ == "__main__":
    import argparse
    import json

    parser = argparse.ArgumentParser(description="Extract GCC optimization passes from dump files")
    parser.add_argument("dump_dir", help="Directory containing GCC dump files")
    parser.add_argument("--source", help="Optional source file basename to filter")
    parser.add_argument("--output", help="Write result as JSON to this file")
    args = parser.parse_args()

    result = extract_gcc_passes(args.dump_dir, args.source)

    if args.output:
        with open(args.output, "w", encoding="utf-8") as f:
            json.dump(result, f, indent=2)
    else:
        print(json.dumps(result, indent=2))
