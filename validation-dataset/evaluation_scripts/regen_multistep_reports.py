#!/usr/bin/env python3
"""Re-evaluate multi-step optimizations using the best candidate.

This helper script runs ``benchmark_experiment.py`` in evaluation-only mode so
existing multi-step solutions are measured again with the ``best.cpp`` logic.
It does not invoke the optimizer itself.
"""

import argparse
import subprocess
from pathlib import Path
import sys

REPO_ROOT = Path(__file__).resolve().parents[1]


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Regenerate reports from existing multi-step optimizations"
    )
    parser.add_argument(
        "--model",
        default="gpt-4o-mini",
        help="Model name used for the optimization runs",
    )
    parser.add_argument(
        "--solutions-root",
        help="Directory with multi-step optimization outputs",
    )
    parser.add_argument(
        "--benchmarks-json",
        default="existing_benchmarks.json",
        help="Benchmark metadata mapping",
    )
    parser.add_argument(
        "--multi-step",
        type=int,
        default=10,
        help="Number of refinement steps that were originally generated",
    )
    parser.add_argument(
        "--output",
        default="reports/multi_step_optimized_code_performance.json",
        help="Summary JSON to generate",
    )
    args = parser.parse_args()

    sol_root = args.solutions_root
    if sol_root is None:
        sol_root = (
            Path("llm_outputs") / "multi_step_optimized_code" / args.model
        )
    else:
        sol_root = Path(sol_root)

    script = REPO_ROOT / "evaluation_scripts" / "benchmark_experiment.py"
    cmd = [
        sys.executable,
        str(script),
        str(sol_root),
        "--benchmarks-json",
        str(args.benchmarks_json),
        "--max-k",
        "1",
        "--multi-step",
        str(args.multi_step),
        "--output",
        str(args.output),
        "--model",
        args.model,
    ]

    subprocess.check_call(cmd, cwd=REPO_ROOT)


if __name__ == "__main__":
    main()
