#!/usr/bin/env python3
"""Run a detailed patch alignment analysis for each benchmark.

This script uses a two step LLM process. The first step gathers three high
temperature analyses of how the benchmark aligns with its reference patch.
Those free form responses are then fed into a second prompt that assigns
numerical alignment scores. A separate classification prompt evaluates the
optimization scenario. All results are written to per-benchmark JSON files.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
import sys

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import (
    call_llm,
    ContextLengthError,
)  # noqa: E402
from llm_integration.compare_versions import (
    _parse_json_response,
    get_default_model,
)  # noqa: E402
from scripts.utilities import read_text, write_text  # noqa: E402

STEP1_PROMPT_FILE = (
    Path(__file__).resolve().parent
    / "prompts"
    / "patch_alignment_step1.prompt"
)
STEP2_PROMPT_FILE = (
    Path(__file__).resolve().parent
    / "prompts"
    / "patch_alignment_step2.prompt"
)
CLASS_PROMPT_FILE = (
    Path(__file__).resolve().parent
    / "prompts"
    / "benchmark_classification.prompt"
)

STEP1_TEMPLATE = read_text(STEP1_PROMPT_FILE)
STEP2_TEMPLATE = read_text(STEP2_PROMPT_FILE)
CLASSIFICATION_TEMPLATE = read_text(CLASS_PROMPT_FILE)


def build_step1_prompt(
    bench: str, orig: str, opt: str, patch: str, before_after: str
) -> str:
    return STEP1_TEMPLATE.format(
        bench=bench,
        orig=orig,
        opt=opt,
        patch=patch,
        before_after=before_after,
    )


def build_classification_prompt(
    bench: str, orig: str, opt: str, patch: str, before_after: str
) -> str:
    return CLASSIFICATION_TEMPLATE.format(
        bench=bench,
        orig=orig,
        opt=opt,
        patch=patch,
        before_after=before_after,
    )


def build_step2_prompt(
    bench: str,
    orig: str,
    opt: str,
    patch: str,
    before_after: str,
    analysis: str,
    step1_prompt: str,
    ) -> str:
        return STEP2_TEMPLATE.format(
            bench=bench,
            orig=orig,
            opt=opt,
            patch=patch,
            before_after=before_after,
            analysis=analysis,
            step1_prompt=step1_prompt,
        )


def parse_scores_and_recommendation(text: str) -> tuple[dict, str]:
    """Return JSON scores and recommendation text from LLM output."""
    if not text or not text.strip():
        raise ValueError("empty response")

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        json_str = fenced.group(1)
        remainder = text[fenced.end():].strip()
    else:
        decoder = json.JSONDecoder()
        idx = text.find("{")
        while idx != -1:
            try:
                obj, end = decoder.raw_decode(text, idx)
                json_str = text[idx:end]
                remainder = text[end:].strip()
                break
            except json.JSONDecodeError:
                idx = text.find("{", idx + 1)
        else:
            raise json.JSONDecodeError("No valid JSON object found", text, 0)

    json_str = re.sub(r",\s*(?=[}\]])", "", json_str)
    scores = json.loads(json_str)
    return scores, remainder


def analyze_benchmark(bench_dir: Path, model: str) -> dict:
    orig_path = bench_dir / "original.cpp"
    opt_path = bench_dir / "optimized.cpp"
    patch_files = list(bench_dir.glob("*.patch"))
    if not patch_files or not orig_path.exists() or not opt_path.exists():
        msg = "required files missing"
        print(f"Skipping {bench_dir}: {msg}")
        return {"error": msg}

    orig_code = read_text(orig_path)
    opt_code = read_text(opt_path)
    patch_text = "\n\n".join(read_text(p) for p in patch_files)

    before_files = {p.stem: p for p in bench_dir.glob("*.before")}
    after_files = {p.stem: p for p in bench_dir.glob("*.after")}
    before_after_parts = []
    for stem, before_path in sorted(before_files.items()):
        after_path = after_files.get(stem)
        if not after_path:
            continue
        before_after_parts.append(
            f"// {before_path.name}\n{read_text(before_path)}\n\n"
            f"// {after_path.name}\n{read_text(after_path)}"
        )
    before_after_text = "\n\n".join(before_after_parts)

    step1_prompt = build_step1_prompt(
        bench_dir.name,
        orig_code,
        opt_code,
        patch_text,
        before_after_text,
    )
    step1_responses: list[str] = []
    for _ in range(3):
        try:
            resp = call_llm(step1_prompt, model=model, temperature=1.2)
        except ContextLengthError:
            msg = "context length exceeded in step 1"
            print(f"Skipping {bench_dir.name}: {msg}")
            return {"error": msg}
        except Exception as exc:
            msg = f"step 1 failed: {exc}"
            print(f"Skipping {bench_dir.name}: {msg}")
            return {"error": msg}
        step1_responses.append(resp.strip())

    step2_prompt = build_step2_prompt(
        bench_dir.name,
        orig_code,
        opt_code,
        patch_text,
        before_after_text,
        "\n\n".join(step1_responses),
        step1_prompt,
    )
    try:
        step2_resp = call_llm(step2_prompt, model=model)
        step2_data, recommendation = parse_scores_and_recommendation(step2_resp)
    except Exception as exc:
        msg = f"step 2 failed: {exc}"
        print(f"Skipping {bench_dir.name}: {msg}")
        return {"error": msg, "step1_responses": step1_responses}

    class_prompt = build_classification_prompt(
        bench_dir.name,
        orig_code,
        opt_code,
        patch_text,
        before_after_text,
    )
    try:
        class_resp = call_llm(class_prompt, model=model)
        classification_data = _parse_json_response(class_resp)
    except Exception as exc:
        print(f"Classification failed for {bench_dir.name}: {exc}")
        classification_data = {}

    result: dict = {
        "step1_responses": step1_responses,
        "scores": step2_data,
        "recommendation": recommendation,
        "classification": classification_data,
    }
    return result


def load_benchmarks_file(path: str) -> list[str]:
    with open(path, "r", encoding="utf-8") as f:
        data = json.load(f)
    benches: list[str] = []
    for item in data.get("benchmarks", []):
        name = item.get("name") if isinstance(item, dict) else item
        if name:
            benches.append(name)
    return benches


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Check benchmark alignment with patches via LLM"
    )
    parser.add_argument("--benchmark", help="Single benchmark to analyze")
    parser.add_argument(
        "--benchmarks-file", help="JSON file listing benchmarks to analyze"
    )
    parser.add_argument(
        "--model",
        default=get_default_model(),
        help="LLM model name",
    )
    parser.add_argument(
        "--output-dir",
        default="reports/patch_alignment",
        help="Directory to write per-benchmark alignment results",
    )
    args = parser.parse_args()

    if args.benchmark and args.benchmarks_file:
        parser.error("Specify --benchmark or --benchmarks-file, not both")

    if args.benchmarks_file:
        bench_names = load_benchmarks_file(args.benchmarks_file)
    elif args.benchmark:
        bench_names = [args.benchmark]
    else:
        bench_names = sorted(
            p.name
            for p in (REPO_ROOT / "benchmarks").iterdir()
            if p.is_dir()
        )

    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    any_results = False
    for name in bench_names:
        bench_dir = REPO_ROOT / "benchmarks" / name
        data = analyze_benchmark(bench_dir, args.model)
        if not data:
            continue
        out_file = out_dir / f"{name}.json"
        write_text(out_file, json.dumps(data, indent=2) + "\n")
        print(f"Saved alignment results to {out_file}")
        any_results = True

    if not any_results:
        print("No results generated")


if __name__ == "__main__":
    main()
