#!/usr/bin/env python3
"""Summarize multi-step benchmark performance.

This script parses ``attempts.json`` files under a multi-step output
 directory and writes a consolidated JSON report containing the best
 LLM result for each benchmark along with baseline and human optimized
 timings.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any, Dict, Optional


def _classify(entry: Dict[str, Any]) -> str:
    """Return status code for an attempt."""
    if not entry.get("compile_success", False):
        return "CE"
    if not entry.get("correctness_passed", False):
        return "CO"
    if not entry.get("runtime_success", False):
        return "RE"
    return "OK"


def _process_dir(bench_dir: Path) -> Dict[str, Any]:
    """Return summary for one benchmark directory."""
    path = bench_dir / "attempts.json"
    with path.open() as f:
        attempts = json.load(f)

    original: Optional[Dict[str, float]] = None
    human: Optional[Dict[str, float]] = None
    best: Optional[Dict[str, float]] = None
    best_step: Optional[int] = None
    status: Optional[str] = None

    for entry in attempts:
        desc = entry.get("description")
        step = entry.get("step")
        if desc == "baseline" or step == 0:
            original = {
                "mean_ms": entry.get("exec_time_mean_ms"),
                "std_ms": entry.get("exec_time_std_ms"),
            }
            continue
        if desc == "human_optimized":
            human = {
                "mean_ms": entry.get("exec_time_mean_ms"),
                "std_ms": entry.get("exec_time_std_ms"),
            }
            continue
        if entry.get("skipped"):
            continue
        label = _classify(entry)
        if label != "OK":
            if status is None:
                status = label
            continue
        mean = entry.get("execution_time_mean_ms")
        if mean is None:
            continue
        if best is None or mean < best.get("mean_ms", float("inf")):
            best = {
                "mean_ms": mean,
                "std_ms": entry.get("execution_time_std_ms"),
            }
            best_step = step
            status = "OK"

    result: Dict[str, Any] = {
        "name": bench_dir.name,
        "original": original,
        "human_optimized": human,
        "status": status or "UNKNOWN",
    }
    if best is not None:
        result["best_llm"] = {"step": best_step, **best}
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description="Collect multi-step benchmark results")
    parser.add_argument(
        "--root",
        default="llm_outputs/multi_step_benchmarks_minibench30_no_hint_V1_attemp_1/gpt-4o-mini",
        help="Directory with multi-step outputs",
    )
    parser.add_argument(
        "--output",
        default="reports/multi_step_benchmarks_minibench30_runtime_summary.json",
        help="File to write aggregated results",
    )
    args = parser.parse_args()

    root = Path(args.root)
    results = []
    for entry in sorted(root.iterdir()):
        if not entry.is_dir():
            continue
        attempts_file = entry / "attempts.json"
        if not attempts_file.exists():
            continue
        results.append(_process_dir(entry))

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        json.dump({"benchmarks": results}, f, indent=2)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
