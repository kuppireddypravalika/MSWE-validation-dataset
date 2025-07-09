#!/usr/bin/env python3
"""Aggregate per-benchmark JSON reports into a single CSV summary.

Each JSON report includes performance statistics, compiler optimizations,
GCC pass frequency data and explicit transformation counts. This script collates that information into a CSV file
with one row per benchmark.

Three columns capture the raw pass counts for the original, baseline and LLM
builds (``gcc_pass_counts_original``, ``gcc_pass_counts_baseline`` and
``gcc_pass_counts_llm``). Two additional columns summarize per-pass
differences: ``gcc_pass_diffs_baseline_vs_orig`` lists deltas between the
baseline and original builds, while ``gcc_pass_diffs_llm_vs_baseline`` compares
the LLM build against the baseline.  The script also records the **total**
difference in pass counts for each comparison using the columns
``gcc_pass_total_diff_baseline_vs_orig`` and
``gcc_pass_total_diff_llm_vs_baseline``.

Similarly, optimization transformation counts are summarized in the columns
``opt_counts_*`` along with per-metric differences and totals.  In addition,
each metric parsed from :mod:`scripts.optimization_counter` is recorded in its
own set of columns with counts for the original, baseline and LLM builds and
differences between them.  This allows direct comparison of specific
transformations such as ``common_subexpr_eliminated`` or ``loops_vectorized``
across implementations.
"""

import argparse
import csv
import json
from pathlib import Path
import sys

# Ensure repository root is on the Python path so imports from ``scripts`` work
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from scripts.optimization_counter import GCC_PATTERNS

# Explicit order for compiler optimization metrics.
# These correspond to keys emitted by :mod:`scripts.optimization_counter` when
# parsing compiler logs.  Keeping them in a fixed order guarantees consistent
# CSV column layout regardless of dictionary ordering.
METRICS = [
    "branches_removed",
    "branches_simplified",
    "common_subexpr_eliminated",
    "constants_propagated",
    "dead_stores_removed",
    "functions_cloned",
    "functions_eliminated",
    "functions_inlined",
    "instructions_eliminated",
    "loops_peeled",
    "loops_unrolled",
    "loops_vectorized",
    "memory_accesses_optimized",
    "redundant_load_store_removed",
    "strength_reduced",
    "tail_calls_optimized",
]

assert set(METRICS) <= set(GCC_PATTERNS.keys()), "METRICS mismatch with patterns"


def gather_reports(reports_dir: Path) -> list[dict]:
    """Return a list of dictionaries summarizing each benchmark result."""
    data = []
    for model_dir in sorted(reports_dir.iterdir()):
        if not model_dir.is_dir():
            continue
        model = model_dir.name
        for json_file in sorted(model_dir.glob("*.json")):
            with json_file.open() as f:
                report = json.load(f)

            bench = report.get("benchmark", json_file.stem)
            perf = report.get("performance", {})
            orig = perf.get("original", {})
            base = perf.get("baseline_optimized", {})
            llm = perf.get("llm_optimized", {})
            correct = report.get("correctness_passed", False)
            opts = report.get("compiler_optimizations", {})
            gcc_passes = report.get("gcc_passes", {})
            opt_counts = report.get("optimization_counts", {})

            base_new = [o for o, s in opts.items() if s.get("baseline") and not s.get("original")]
            llm_new = [o for o, s in opts.items() if s.get("llm") and not s.get("original")]

            orig_passes = gcc_passes.get("original", {})
            baseline_passes = gcc_passes.get("baseline", {})
            llm_passes = gcc_passes.get("llm", {})

            orig_opt_counts = opt_counts.get("original", {})
            baseline_opt_counts = opt_counts.get("baseline", {})
            llm_opt_counts = opt_counts.get("llm", {})

            def fmt_opt_counts(counts: dict) -> str:
                return ";".join(f"{k}:{v}" for k, v in sorted(counts.items()))

            opt_diffs_base = []
            for m in sorted(set(orig_opt_counts) | set(baseline_opt_counts)):
                oc = orig_opt_counts.get(m, 0)
                bc = baseline_opt_counts.get(m, 0)
                if oc != bc:
                    opt_diffs_base.append(f"{m}:{bc - oc:+d}")

            opt_diffs_llm = []
            for m in sorted(set(baseline_opt_counts) | set(llm_opt_counts)):
                bc = baseline_opt_counts.get(m, 0)
                lc = llm_opt_counts.get(m, 0)
                if bc != lc:
                    opt_diffs_llm.append(f"{m}:{lc - bc:+d}")

            total_opt_diff_base = sum(
                baseline_opt_counts.get(m, 0) - orig_opt_counts.get(m, 0)
                for m in set(orig_opt_counts) | set(baseline_opt_counts)
            )
            total_opt_diff_llm = sum(
                llm_opt_counts.get(m, 0) - baseline_opt_counts.get(m, 0)
                for m in set(baseline_opt_counts) | set(llm_opt_counts)
            )

            def fmt_pass_counts(passes: dict) -> str:
                parts = [f"{p}:{info.get('count', 0)}" for p, info in sorted(passes.items())]
                return ";".join(parts)

            pass_diffs_base = []
            for p in sorted(set(orig_passes) | set(baseline_passes)):
                ocnt = orig_passes.get(p, {}).get("count", 0)
                bcnt = baseline_passes.get(p, {}).get("count", 0)
                if ocnt != bcnt:
                    pass_diffs_base.append(f"{p}:{bcnt - ocnt:+d}")

            pass_diffs_llm = []
            for p in sorted(set(baseline_passes) | set(llm_passes)):
                bcnt = baseline_passes.get(p, {}).get("count", 0)
                lcnt = llm_passes.get(p, {}).get("count", 0)
                if bcnt != lcnt:
                    pass_diffs_llm.append(f"{p}:{lcnt - bcnt:+d}")

            total_diff_base = sum(
                (
                    baseline_passes.get(p, {}).get("count", 0)
                    - orig_passes.get(p, {}).get("count", 0)
                )
                for p in set(orig_passes) | set(baseline_passes)
            )
            total_diff_llm = sum(
                (
                    llm_passes.get(p, {}).get("count", 0)
                    - baseline_passes.get(p, {}).get("count", 0)
                )
                for p in set(baseline_passes) | set(llm_passes)
            )

            orig_count_str = fmt_pass_counts(orig_passes)
            baseline_count_str = fmt_pass_counts(baseline_passes)
            llm_count_str = fmt_pass_counts(llm_passes)

            orig_opt_str = fmt_opt_counts(orig_opt_counts)
            baseline_opt_str = fmt_opt_counts(baseline_opt_counts)
            llm_opt_str = fmt_opt_counts(llm_opt_counts)

            row = {
                "model": model,
                "benchmark": bench,
                "orig_mean": orig.get("mean"),
                "orig_std": orig.get("std"),
                "baseline_mean": base.get("mean"),
                "baseline_std": base.get("std"),
                "llm_mean": llm.get("mean"),
                "llm_std": llm.get("std"),
                "speedup_baseline_vs_orig": (orig.get("mean") / base.get("mean")
                                            if orig.get("mean") and base.get("mean") else float("nan")),
                "speedup_llm_vs_orig": (orig.get("mean") / llm.get("mean")
                                       if orig.get("mean") and llm.get("mean") else float("nan")),
                "speedup_llm_vs_baseline": (base.get("mean") / llm.get("mean")
                                           if base.get("mean") and llm.get("mean") else float("nan")),
                "correctness_passed": correct,
                "baseline_new_opts": ",".join(sorted(base_new)),
                "llm_new_opts": ",".join(sorted(llm_new)),
                "gcc_pass_counts_original": orig_count_str,
                "gcc_pass_counts_baseline": baseline_count_str,
                "gcc_pass_counts_llm": llm_count_str,
                "gcc_pass_diffs_baseline_vs_orig": ";".join(pass_diffs_base),
                "gcc_pass_diffs_llm_vs_baseline": ";".join(pass_diffs_llm),
                "gcc_pass_total_diff_baseline_vs_orig": total_diff_base,
                "gcc_pass_total_diff_llm_vs_baseline": total_diff_llm,
                "opt_counts_original": orig_opt_str,
                "opt_counts_baseline": baseline_opt_str,
                "opt_counts_llm": llm_opt_str,
                "opt_diffs_baseline_vs_orig": ";".join(opt_diffs_base),
                "opt_diffs_llm_vs_baseline": ";".join(opt_diffs_llm),
                "opt_total_diff_baseline_vs_orig": total_opt_diff_base,
                "opt_total_diff_llm_vs_baseline": total_opt_diff_llm,
            }

            # Record per-metric counts and differences
            for metric in METRICS:
                oc = orig_opt_counts.get(metric, 0)
                bc = baseline_opt_counts.get(metric, 0)
                lc = llm_opt_counts.get(metric, 0)
                row[f"{metric}_orig"] = oc
                row[f"{metric}_baseline"] = bc
                row[f"{metric}_llm"] = lc
                row[f"{metric}_diff_baseline_vs_orig"] = bc - oc
                row[f"{metric}_diff_llm_vs_baseline"] = lc - bc

            data.append(row)
    return data


def write_csv(rows: list[dict], out_path: Path) -> None:
    """Write aggregated rows to a CSV file."""
    fieldnames = [
        "model",
        "benchmark",
        "orig_mean",
        "orig_std",
        "baseline_mean",
        "baseline_std",
        "llm_mean",
        "llm_std",
        "speedup_baseline_vs_orig",
        "speedup_llm_vs_orig",
        "speedup_llm_vs_baseline",
        "correctness_passed",
        "baseline_new_opts",
        "llm_new_opts",
        "gcc_pass_counts_original",
        "gcc_pass_counts_baseline",
        "gcc_pass_counts_llm",
        "gcc_pass_diffs_baseline_vs_orig",
        "gcc_pass_diffs_llm_vs_baseline",
        "gcc_pass_total_diff_baseline_vs_orig",
        "gcc_pass_total_diff_llm_vs_baseline",
        "opt_counts_original",
        "opt_counts_baseline",
        "opt_counts_llm",
        "opt_diffs_baseline_vs_orig",
        "opt_diffs_llm_vs_baseline",
        "opt_total_diff_baseline_vs_orig",
        "opt_total_diff_llm_vs_baseline",
    ]
    # Add columns for each optimization metric
    for metric in METRICS:
        fieldnames.extend(
            [
                f"{metric}_orig",
                f"{metric}_baseline",
                f"{metric}_llm",
                f"{metric}_diff_baseline_vs_orig",
                f"{metric}_diff_llm_vs_baseline",
            ]
        )
    with out_path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)
    print(f"Summary written to {out_path}")


def main() -> None:
    """Parse arguments and generate the aggregated CSV report."""
    parser = argparse.ArgumentParser(description="Aggregate JSON benchmark reports")
    parser.add_argument(
        "--reports-dir", default="reports", help="Directory containing JSON reports"
    )
    parser.add_argument(
        "--output", default="reports/accumulated_report.csv", help="Output CSV file"
    )
    args = parser.parse_args()

    reports_dir = Path(args.reports_dir)
    rows = gather_reports(reports_dir)
    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    write_csv(rows, out_path)


if __name__ == "__main__":
    main()
