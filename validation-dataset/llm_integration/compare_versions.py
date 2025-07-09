#!/usr/bin/env python3
"""Compare optimized code versions using GPT analysis.

This tool gathers the original benchmark code, the hand‑optimized
implementation and the top performing LLM-generated versions, then
queries an LLM for a structured comparison. By default it analyzes the
most recent benchmark listed in ``reports/benchmark_performance.json``
but it can also iterate over a set of benchmarks provided via
``--benchmarks-file``. Results are written under
``reports/code_comparisons/<model>/<benchmark>_<solution>.json``.
"""

import argparse
import json
import re
from pathlib import Path
import sys

# Ensure repository root is on the Python path so we can import from scripts/
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import ensure_dir, read_text, write_text


def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from LLM text output."""

    if not text or not text.strip():
        raise ValueError("empty response")

    # Grab fenced JSON block if present
    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        text = fenced.group(1)

    # Remove trailing commas before closing braces/brackets as LLMs sometimes
    # produce them. This keeps parsing logic simple without needing extra deps.
    text = re.sub(r",\s*(?=[}\]])", "", text)

    decoder = json.JSONDecoder()
    idx = text.find("{")
    while idx != -1:
        try:
            obj, _ = decoder.raw_decode(text, idx)
            return obj
        except json.JSONDecodeError:
            idx = text.find("{", idx + 1)

    raise json.JSONDecodeError("No valid JSON object found", text, 0)


def _find_value(obj: object, keys: set[str]):
    """Recursively search ``obj`` for the first matching key in ``keys``."""
    if isinstance(obj, dict):
        for k, v in obj.items():
            if k in keys:
                return v
            res = _find_value(v, keys)
            if res is not None:
                return res
    elif isinstance(obj, list):
        for v in obj:
            res = _find_value(v, keys)
            if res is not None:
                return res
    return None


def normalize_result(data: dict) -> dict:
    """Return ``data`` mapped to a consistent schema."""

    def to_bool(val: object) -> bool | None:
        if isinstance(val, bool):
            return val
        if isinstance(val, str):
            low = val.strip().lower()
            if low in {"yes", "true", "y", "1"}:
                return True
            if low in {"no", "false", "n", "0"}:
                return False
        return None

    same = _find_value(
        data,
        {
            "same_optimizations",
            "hand_optimized_equals_llm",
            "is_hand_optimized_equivalent_to_llm",
            "hand_optimized_equivalence",
            "hand_optimized_equivalence_to_llm",
            "hand_optimized_matches_llm",
            "1",
            "question_1",
        },
    )
    if isinstance(same, dict):
        same = same.get("same_optimizations") or same.get("answer")
    same = to_bool(same)

    missing = _find_value(
        data,
        {
            "missing_optimizations",
            "missing_optimizations_in_llm",
            "missing_hand_optimizations_in_llm",
            "missing_hand_optimizations",
            "missing_optimizations_hand",
            "2",
            "question_2",
        },
    )
    if isinstance(missing, dict):
        missing = missing.get("missing_optimizations") or missing.get("answer")
    if missing is None:
        missing = []
    elif isinstance(missing, str):
        missing = [missing]

    additional = _find_value(
        data,
        {
            "new_optimizations",
            "additional_optimizations",
            "llm_additional_optimizations",
            "additional_optimizations_in_llm",
            "additional_optimizations_llm",
            "optimizations_in_llm_beyond_hand",
            "extra_optimizations",
            "llm_optimizations_over_hand",
            "3",
            "question_3",
        },
    )
    if isinstance(additional, dict):
        additional = (
            additional.get("additional_optimizations")
            or additional.get("extra_optimizations")
            or additional.get("optimizations")
            or additional.get("answer")
        )
    if additional is None:
        additional = []
    elif isinstance(additional, str):
        additional = [additional]

    reason = _find_value(
        data,
        {
            "reasons_for_missed_optimizations",
            "reasons_for_missing_optimizations",
            "possible_reasons_llm_missed_hand_optimizations",
            "possible_reasons_for_missing_hand_optimizations",
            "explanation_for_missing_optimizations",
            "explanation_for_llm_missed_optimizations",
            "reasoning_for_llm_missed_optimizations",
            "reasoning_for_missed_optimizations",
            "possible_reason_for_missed_optimizations",
            "explanation",
            "reasoning",
            "4",
            "question_4",
        },
    )
    if isinstance(reason, dict):
        reason = reason.get("explanation") or reason.get("reasoning") or reason.get("answer")
    if isinstance(reason, list):
        reason = " ".join(str(r) for r in reason)
    if reason is None:
        reason = ""

    insights = _find_value(
        data,
        {
            "additional_insights",
            "insights",
            "llm_optimization_insights",
        },
    )
    if isinstance(insights, dict):
        insights = " ".join(str(v) for v in insights.values())
    if isinstance(insights, list):
        insights = " ".join(str(v) for v in insights)
    if insights is None:
        insights = ""

    bypass = _find_value(
        data,
        {
            "bypass_performance_benchmark",
            "performance_benchmark_violation",
            "llm_bypassed_performance_benchmark",
            "5",
            "question_5",
        },
    )
    if isinstance(bypass, dict):
        bypass = bypass.get("answer")
    bypass = to_bool(bypass)

    validity = _find_value(
        data,
        {
            "performance_test_validity",
            "validity_of_performance_test",
            "performance_test_case_validity",
            "6",
            "question_6",
        },
    )
    if isinstance(validity, dict):
        validity = validity.get("answer") or validity.get("explanation")
    if isinstance(validity, list):
        validity = " ".join(str(v) for v in validity)
    if validity is None:
        validity = ""

    return {
        "same_optimizations": same,
        "missing_optimizations": missing,
        "additional_optimizations": additional,
        "reasons_for_missed_optimizations": reason,
        "additional_insights": insights,
        "bypass_performance_benchmark": bypass,
        "performance_test_validity": validity,
    }


def load_metadata(
    benchmark: str, top_n: int = 2
) -> tuple[str, str, dict, list[str], dict[str, dict]]:
    """Return inefficiency, optimization, performance data and solution metrics."""

    ineff = ""
    opt = ""
    perf: dict = {}
    top_ids: list[str] = []
    exec_info: dict[str, dict] = {}

    bench_map = Path("existing_benchmarks.json")
    if bench_map.exists():
        with bench_map.open() as f:
            data = json.load(f)
        for item in data.get("benchmarks", []):
            if item.get("name") == benchmark:
                ineff = item.get("inefficiency", "")
                opt = item.get("optimization", "")
                break

    perf_path = Path("reports/benchmark_performance.json")
    if perf_path.exists():
        with perf_path.open() as f:
            data = json.load(f)
        for item in data.get("benchmarks", []):
            bid = item.get("benchmark_id") or item.get("name")
            if bid == benchmark:
                orig = (item.get("original_performance") or {}).get("mean")
                human = (item.get("human_performance") or {}).get("mean")
                llm_solutions = item.get("llm_solutions", [])
                runnable = [
                    s
                    for s in llm_solutions
                    if s.get("runnable")
                    and s.get("performance")
                    and s["performance"].get("mean") is not None
                ]
                runnable.sort(key=lambda s: s["performance"]["mean"])
                for s in runnable[:top_n]:
                    sid = s.get("solution_id")
                    if sid:
                        top_ids.append(sid)
                for s in llm_solutions:
                    sid = s.get("solution_id")
                    if not sid:
                        continue
                    exec_info[sid] = {
                        "runnable": s.get("runnable"),
                        "performance": s.get("performance"),
                    }
                if runnable and orig:
                    best = runnable[0]["performance"]["mean"]
                    perf["llm_over_original"] = orig / best if best else None
                if orig and human:
                    perf["baseline_over_original"] = orig / human if human else None
                break

    return ineff, opt, perf, top_ids, exec_info


def get_default_benchmark(perf_path: Path = Path("reports/benchmark_performance.json")) -> str:
    """Return the last benchmark listed in benchmark_performance.json."""
    if not perf_path.exists():
        raise SystemExit(f"{perf_path} not found")

    with perf_path.open() as f:
        data = json.load(f)

    benches = data.get("benchmarks", [])
    if not benches:
        raise SystemExit(f"No benchmarks listed in {perf_path}")
    last = benches[-1]
    return last.get("benchmark_id") or last.get("name")


def get_default_model(perf_path: Path = Path("reports/benchmark_performance.json")) -> str:
    """Return the model name recorded in benchmark_performance.json."""
    if not perf_path.exists():
        raise SystemExit(f"{perf_path} not found")

    with perf_path.open() as f:
        data = json.load(f)

    model = data.get("model")
    if not model:
        raise SystemExit(f"No model specified in {perf_path}")
    return model


def default_paths(benchmark: str, model: str) -> tuple[Path, Path]:
    """Return benchmark directory and LLM output directory for benchmark."""
    bench_dir = Path("benchmarks") / benchmark
    llm_dir = (
        Path("llm_outputs")
        / "downstream_task_1_optimized_code"
        / model
        / benchmark
    )
    return bench_dir, llm_dir


def build_prompt(
    bench: str,
    ineff: str,
    opt: str,
    orig: str,
    hand: str,
    llm: str,
    perf: dict,
) -> str:
    """Construct the analysis prompt for the LLM."""
    return f"""You are analyzing optimization results for benchmark '{bench}'.
Inefficiency: {ineff}
Hand optimization: {opt}

Original implementation:\n{orig}

Hand optimized implementation:\n{hand}

LLM optimized implementation:\n{llm}

Performance data (speedup ratios):\n{json.dumps(perf)}

Provide a JSON object answering these questions:
1. Does the hand optimized code implement the same optimizations as the LLM version?
2. What optimizations from the hand version are missing in the LLM version?
3. Did the LLM version introduce optimizations beyond the hand version? If so, list them.
4. Explain possible reasons the LLM missed optimizations present in the hand code.
5. Did the LLM generated code violate or bypass the performance benchmark?
6. If the hand optimized code executes extremely quickly (near zero time), is the performance test scenario still valid? Explain.
Include any additional insights about the optimization strategies.

Return **only** a JSON object with this schema:
{{
  "same_optimizations": <true|false>,
  "missing_optimizations": ["..."],
  "additional_optimizations": ["..."],
  "reasons_for_missed_optimizations": "...",
  "additional_insights": "...",
  "bypass_performance_benchmark": <true|false>,
  "performance_test_validity": "..."
}}
"""


def compare_single_benchmark(
    bench: str,
    model: str,
    bench_dir: Path | None = None,
    llm_file: Path | None = None,
) -> None:
    """Generate comparison reports for the best LLM solutions."""

    bench_path_default, llm_dir_default = default_paths(bench, model)
    bench_path = Path(bench_dir) if bench_dir else bench_path_default

    orig_path = bench_path / "original.cpp"
    hand_path = bench_path / "optimized.cpp"

    if not (orig_path.exists() and hand_path.exists()):
        print(f"Skipping {bench}: required source files not found")
        return

    if llm_file and Path(llm_file).is_file():
        llm_dir = Path(llm_file).parent
        solution_ids = [Path(llm_file).stem]
        ineff, opt, perf, _, exec_info = load_metadata(bench)
    else:
        llm_dir = Path(llm_file) if llm_file else llm_dir_default
        ineff, opt, perf, solution_ids, exec_info = load_metadata(bench)
        if not solution_ids:
            solution_ids = ["optimized"]

    orig_code = read_text(orig_path)
    hand_code = read_text(hand_path)

    out_dir = Path("reports") / "code_comparisons" / model
    ensure_dir(out_dir)

    max_solutions = 2 if (not llm_file or not Path(llm_file).is_file()) else len(solution_ids)
    for sid in solution_ids[:max_solutions]:
        if sid == "optimized":
            llm_path = llm_dir / "optimized.cpp"
            file_suffix = ""
        else:
            idx_match = re.search(r"_(\d+)$", sid)
            idx = idx_match.group(1) if idx_match else sid
            llm_path = llm_dir / f"optimized_{idx}.cpp"
            file_suffix = f"_{idx}"

        if not llm_path.exists():
            print(f"Skipping {bench} solution {sid}: file not found")
            continue

        out_file = out_dir / f"{bench}{file_suffix}.json"
        if out_file.exists():
            print(f"Skipping {bench} solution {sid}: analysis already exists")
            continue

        llm_code = read_text(llm_path)
        prompt = build_prompt(bench, ineff, opt, orig_code, hand_code, llm_code, perf)

        try:
            response = call_llm(prompt, model=model)
        except ContextLengthError:
            print(f"Skipping {bench} solution {sid}: context length exceeded")
            continue

        try:
            parsed = _parse_json_response(response)
        except Exception as exc:
            raise RuntimeError(
                f"Failed to parse LLM response: {exc}\nResponse was:\n{response}"
            ) from exc

        result = normalize_result(parsed)
        perf_summary = dict(perf)
        if sid in exec_info:
            perf_summary["execution"] = exec_info[sid]
        result["performance"] = perf_summary
        result["solution_id"] = sid

        write_text(out_file, json.dumps(result, indent=2))
        print(f"Saved analysis to {out_file}")


def parse_args() -> argparse.Namespace:
    """Return CLI arguments with defaults derived from repo data."""
    default_model = get_default_model()
    default_bench = get_default_benchmark()
    bench_dir_default, llm_dir_default = default_paths(default_bench, default_model)

    parser = argparse.ArgumentParser(
        description="Compare human and LLM optimizations",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "--benchmark",
        default=default_bench,
        help=f"Name of benchmark to analyze (default: {default_bench})",
    )
    parser.add_argument(
        "--bench-dir",
        help=f"Path to benchmark directory (default: {bench_dir_default})",
    )
    parser.add_argument(
        "--llm-file",
        help=(
            "Path to LLM optimized code file or directory "
            f"(default: {llm_dir_default})"
        ),
    )
    parser.add_argument(
        "--model",
        default=default_model,
        help=f"LLM model name (default: {default_model})",
    )
    parser.add_argument(
        "--benchmarks-file",
        help="JSON file listing benchmarks to process",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    if args.benchmarks_file:
        with open(args.benchmarks_file, "r", encoding="utf-8") as f:
            data = json.load(f)
        benchmarks = []
        for item in data.get("benchmarks", []):
            if isinstance(item, dict):
                name = item.get("name")
            else:
                name = item
            if name:
                benchmarks.append(name)
    else:
        bench = args.benchmark
        if args.bench_dir is None or args.llm_file is None:
            bdir, ldir = default_paths(bench, args.model)
            if args.bench_dir is None:
                args.bench_dir = str(bdir)
            if args.llm_file is None:
                args.llm_file = str(ldir)
        benchmarks = [bench]

    for bench in benchmarks:
        if args.benchmarks_file:
            compare_single_benchmark(bench, args.model)
        else:
            compare_single_benchmark(bench, args.model, args.bench_dir, args.llm_file)


if __name__ == "__main__":
    main()
