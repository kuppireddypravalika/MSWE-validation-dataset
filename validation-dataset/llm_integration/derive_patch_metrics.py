#!/usr/bin/env python3
"""Derive patch metrics via LLM prompting.

This script analyzes a directory containing ``.patch``, ``.before`` and
``.after`` files and queries an LLM to obtain alignment scores. It
returns the ``performance_optimization_relevance`` and
``contextual_dependency`` metrics along with a new
``library_api_dependency`` metric indicating whether a library or API
call was involved and how generic that library is. The script also
extracts the definitions and few-shot examples for these metrics from
``patch_alignment_step1.prompt`` so the caller can inspect the exact
wording used in the prompt.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
import sys

# Ensure repository root is on the Python path
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import read_text, write_text

MAX_LINES = 500


def file_line_count(path: Path) -> int:
    """Return the number of lines in *path*."""
    with open(path, "r", encoding="utf-8") as f:
        return sum(1 for _ in f)

STEP1_FILE = Path(__file__).resolve().parent / "prompts" / "patch_alignment_step1.prompt"
STEP2_FILE = Path(__file__).resolve().parent / "prompts" / "patch_alignment_step2.prompt"


def extract_metric_snippets() -> tuple[str, str, str]:
    """Return the definition and examples for the three metrics from the prompt."""
    text = read_text(STEP1_FILE)
    perf_match = re.search(
        r"Performance Optimization Relevance \(1-5\):\n(.*?)(?=\n\s*Contextual Dependency)",
        text,
        re.DOTALL,
    )
    ctx_match = re.search(
        r"Contextual Dependency \(Local vs\. Repository\) \(1-5\):\n(.*?)(?=\n\s*Library/API Dependency)",
        text,
        re.DOTALL,
    )
    lib_match = re.search(
        r"Library/API Dependency \(0-5, Yes/No\):\n(.*?)(?=\n\s*Important Note)",
        text,
        re.DOTALL,
    )
    perf_snippet = perf_match.group(1).strip() if perf_match else ""
    ctx_snippet = ctx_match.group(1).strip() if ctx_match else ""
    lib_snippet = lib_match.group(1).strip() if lib_match else ""
    return perf_snippet, ctx_snippet, lib_snippet


def build_step1_prompt(bench: str, patch: str, before_after: str) -> str:
    template = read_text(STEP1_FILE)
    return template.format(bench=bench, orig="", opt="", patch=patch, before_after=before_after)


def build_step2_prompt(bench: str, analysis: str, step1_prompt: str, patch: str, before_after: str) -> str:
    template = read_text(STEP2_FILE)
    return template.format(
        bench=bench,
        orig="",
        opt="",
        patch=patch,
        before_after=before_after,
        analysis=analysis,
        step1_prompt=step1_prompt,
    )


def parse_scores(text: str) -> dict:
    if not text or not text.strip():
        raise ValueError("empty response")

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        json_str = fenced.group(1)
    else:
        decoder = json.JSONDecoder()
        idx = text.find("{")
        while idx != -1:
            try:
                obj, end = decoder.raw_decode(text, idx)
                json_str = text[idx:end]
                break
            except json.JSONDecodeError:
                idx = text.find("{", idx + 1)
        else:
            raise json.JSONDecodeError("No valid JSON object found", text, 0)

    json_str = re.sub(r",\s*(?=[}\]])", "", json_str)
    return json.loads(json_str)


def gather_before_after(dir_path: Path) -> str:
    before_files = {p.stem: p for p in dir_path.glob("*.before")}
    after_files = {p.stem: p for p in dir_path.glob("*.after")}
    parts = []
    for stem, before in sorted(before_files.items()):
        after = after_files.get(stem)
        if not after:
            continue
        if file_line_count(before) >= MAX_LINES or file_line_count(after) >= MAX_LINES:
            raise ValueError(f"{before.name} or {after.name} exceeds {MAX_LINES} lines")
        parts.append(
            f"// {before.name}\n{read_text(before)}\n\n// {after.name}\n{read_text(after)}"
        )
    return "\n\n".join(parts)


def gather_patch(dir_path: Path) -> str:
    patch_files = list(dir_path.glob("*.patch"))
    if not patch_files:
        raise FileNotFoundError("no .patch files found")
    for p in patch_files:
        if file_line_count(p) >= MAX_LINES:
            raise ValueError(f"{p.name} exceeds {MAX_LINES} lines")
    return "\n\n".join(read_text(p) for p in patch_files)


def analyze_benchmark(dir_path: Path, model: str) -> dict:
    """Return patch metric data for ``dir_path``."""
    try:
        patch_text = gather_patch(dir_path)
        before_after = gather_before_after(dir_path)
    except FileNotFoundError as exc:
        return {"error": str(exc)}
    except ValueError as exc:
        return {"skipped": str(exc)}

    step1_prompt = build_step1_prompt(dir_path.name, patch_text, before_after)
    try:
        step1_resp = call_llm(step1_prompt, model=model, temperature=1.2)
    except ContextLengthError:
        return {"error": "step 1 prompt exceeds context length"}

    step2_prompt = build_step2_prompt(
        dir_path.name, step1_resp.strip(), step1_prompt, patch_text, before_after
    )
    step2_resp = call_llm(step2_prompt, model=model)
    try:
        scores = parse_scores(step2_resp)
    except (json.JSONDecodeError, ValueError) as exc:
        return {"error": f"failed to parse scores: {exc}"}

    perf_def, ctx_def, lib_def = extract_metric_snippets()
    return {
        "performance_optimization_relevance": scores.get("performance_optimization_relevance"),
        "contextual_dependency": scores.get("contextual_dependency"),
        "library_api_dependency": scores.get("library_api_dependency"),
        "performance_optimization_relevance_definition": perf_def,
        "contextual_dependency_definition": ctx_def,
        "library_api_dependency_definition": lib_def,
    }


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
    parser = argparse.ArgumentParser(description="Derive patch metrics via LLM")
    parser.add_argument("bench_dir", nargs="?", help="Directory with .patch, .before and .after files")
    parser.add_argument("--benchmarks-file", help="JSON file listing benchmarks to analyze")
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    parser.add_argument("--output-dir", default="reports/patch_metrics", help="Directory to write per-benchmark results")
    args = parser.parse_args()

    if args.bench_dir and args.benchmarks_file:
        parser.error("Specify bench_dir or --benchmarks-file, not both")

    if args.benchmarks_file:
        bench_names = load_benchmarks_file(args.benchmarks_file)
    elif args.bench_dir:
        bench_names = [Path(args.bench_dir).name]
        args.output_dir = "-"  # signal stdout only
    else:
        parser.error("bench_dir is required if --benchmarks-file is not provided")

    out_dir = Path(args.output_dir)
    if out_dir != Path("-"):
        out_dir.mkdir(parents=True, exist_ok=True)

    for name in bench_names:
        bench_dir = Path(args.bench_dir) if args.benchmarks_file is None else REPO_ROOT / "benchmarks" / name
        data = analyze_benchmark(bench_dir, args.model)
        if out_dir == Path("-"):
            print(json.dumps(data, indent=2))
        else:
            out_file = out_dir / f"{name}.json"
            write_text(out_file, json.dumps(data, indent=2) + "\n")
            print(f"Saved metrics to {out_file}")


if __name__ == "__main__":
    main()
