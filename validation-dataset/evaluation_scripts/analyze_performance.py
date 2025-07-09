#!/usr/bin/env python3
"""Analyze benchmark performance JSON and compute metrics."""

import argparse
import json
import platform
from pathlib import Path
from statistics import mean


def _sanitize(name: str) -> str:
    """Return a filesystem-friendly version of a string."""
    return "".join(c if c.isalnum() or c in {"-", "_"} else "_" for c in name)


def main() -> None:
    parser = argparse.ArgumentParser(description="Analyze benchmark performance")
    parser.add_argument(
        "--input",
        default="reports/benchmark_performance.json",
        help="Input benchmark performance JSON",
    )
    parser.add_argument(
        "--variant",
        default="single-step",
        help="Model variant (single-step, multi-step, etc.)",
    )
    parser.add_argument(
        "--output",
        help="Output JSON file; defaults to reports/analysis_results_<model>_<variant>.json",
    )
    args = parser.parse_args()

    with Path(args.input).open() as f:
        data = json.load(f)

    benchmarks = data.get("benchmarks", [])
    model_name = data.get("model", "GPT-4o-mini")

    total_benchmarks = len(benchmarks)
    runnable_solutions = 0
    thresholds = [0.01, 0.05, 0.10, 0.25]
    imp_orig = {str(int(t * 100)): 0 for t in thresholds}
    imp_human = {str(int(t * 100)): 0 for t in thresholds}
    pass_at_k = {1: 0, 3: 0, 5: 0}
    max_at_k_values = {1: [], 3: [], 5: []}

    for bench in benchmarks:
        orig_entry = bench.get("original_performance") or {}
        human_entry = bench.get("human_performance") or {}
        orig_perf = orig_entry.get("mean")
        human_perf = human_entry.get("mean")
        llm_solutions = bench.get("llm_solutions", [])

        if orig_perf is None or human_perf is None or human_perf == 0:
            continue

        runnable = [s for s in llm_solutions if s.get("runnable") and s.get("performance") and s["performance"].get("mean") and s["performance"]["mean"] > 0]
        if runnable:
            runnable_solutions += 1
            best_perf = min(s["performance"]["mean"] for s in runnable)
            if best_perf == 0:
                continue

            improvement_llm = orig_perf / best_perf
            improvement_human = orig_perf / human_perf

            for t in thresholds:
                if improvement_llm > (1 + t):
                    imp_orig[str(int(t * 100))] += 1
                if best_perf < human_perf * (1 - t):
                    imp_human[str(int(t * 100))] += 1

            for k in pass_at_k:
                if any(s.get("runnable") for s in llm_solutions[:k]):
                    pass_at_k[k] += 1

            for k in max_at_k_values:
                vals = [s["performance"]["mean"] for s in llm_solutions[:k] if s.get("runnable") and s.get("performance") and s["performance"].get("mean") and s["performance"]["mean"] > 0]
                if vals:
                    max_at_k_values[k].append(orig_perf / min(vals))

    results = {
        "model": model_name,
        "variant": args.variant,
        "general_performance": {
            "total_benchmarks": total_benchmarks,
            "runnable_solutions_count": runnable_solutions,
            "runnable_solutions_percentage": round((runnable_solutions / total_benchmarks) * 100, 2) if total_benchmarks else 0,
        },
        "improvement_thresholds": {
            "over_original": imp_orig,
            "over_human": imp_human,
        },
        "pass_at_k_percentages": {str(k): round((v / total_benchmarks) * 100, 2) if total_benchmarks else 0 for k, v in pass_at_k.items()},
        "max_at_k_means": {str(k): round(float(mean(v)), 2) if v else None for k, v in max_at_k_values.items()},
        "system_info": {
            **data.get("system", {}),
            "platform": platform.platform(),
            "python_version": platform.python_version(),
            "processor": platform.processor(),
        },
    }

    if args.output:
        out_path = Path(args.output)
    else:
        name = f"analysis_results_{_sanitize(model_name)}_{_sanitize(args.variant)}.json"
        out_path = Path("reports") / name
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        json.dump(results, f, indent=2)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
