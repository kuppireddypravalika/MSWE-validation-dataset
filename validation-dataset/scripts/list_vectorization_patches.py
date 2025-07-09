#!/usr/bin/env python3
"""Detect benchmarks whose patches apply CPU vectorization optimizations.

This utility scans a range of benchmarks and sends their ``*.patch``
content to an LLM using the :doc:`vectorization_detection.prompt`.
The LLM response indicates whether the patch implements any local
performance optimisations and, if so, whether the ``SIMD`` family
(vectorisation) is present. Results for each benchmark are stored in a
single JSON file so repeated runs only evaluate new benchmarks.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
import sys

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm
from scripts.utilities import read_text, write_text

PROMPT_FILE = (
    Path(__file__).resolve().parents[1]
    / "llm_integration"
    / "prompts"
    / "vectorization_detection.prompt"
)


PROMPT_TEMPLATE = Path(PROMPT_FILE).read_text()


def build_prompt(patch_text: str) -> str:
    """Return the final LLM prompt with ``patch_text`` inserted."""
    prompt = PROMPT_TEMPLATE
    prompt = prompt.replace("<Commit-Message>", "vectorization patch")
    prompt = prompt.replace(
        "<Diff – unified “git show --patch” output>", patch_text
    )
    return prompt


def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from an LLM text response."""
    if not text or not text.strip():
        raise ValueError("empty response")

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        text = fenced.group(1)

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


def analyze_patch(patch_text: str, model: str) -> dict | None:
    """Return the LLM JSON result for ``patch_text`` or ``None`` on error."""
    safe_patch = patch_text.replace("{", "{{").replace("}", "}}")
    prompt = build_prompt(safe_patch)
    try:
        resp = call_llm(prompt, model=model)
    except Exception as exc:
        print(f"LLM query failed: {exc}")
        return None
    try:
        return _parse_json_response(resp)
    except Exception:
        print("Could not parse LLM response")
        return None


def patch_uses_vectorization(patch_text: str, model: str) -> tuple[bool, dict | None]:
    """Return (``True`` if SIMD optimisation present, response data)."""
    data = analyze_patch(patch_text, model)
    if not data or not data.get("is_local_optimisation"):
        return False, data

    for fam in data.get("families") or []:
        if fam.get("id") == "SIMD":
            return True, data
    return False, data


def benchmark_has_vectorization(bench_dir: Path, model: str) -> tuple[bool, dict | None]:
    """Return ``(True, data)`` if benchmark uses vectorization."""
    patch_parts = []
    for patch_file in bench_dir.glob("*.patch"):
        try:
            patch_parts.append(read_text(patch_file))
        except Exception:
            continue
    if not patch_parts:
        return False, None
    patch_text = "\n\n".join(patch_parts)
    return patch_uses_vectorization(patch_text, model)


def collect_benchmarks(start: int, end: int, bench_root: Path) -> list[Path]:
    benches: list[Path] = []
    for i in range(start, end + 1):
        p = bench_root / f"benchmark_{i}"
        if p.is_dir():
            benches.append(p)
    return benches


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List benchmarks with vectorization-related patches"
    )
    parser.add_argument("start", type=int, help="First benchmark index (inclusive)")
    parser.add_argument("end", type=int, help="Last benchmark index (inclusive)")
    parser.add_argument(
        "--bench-root",
        default="benchmarks",
        help="Root directory containing benchmark folders",
    )
    parser.add_argument(
        "--model",
        default="gpt-4o-mini",
        help="LLM model name",
    )
    parser.add_argument(
        "--output",
        default="reports/vectorization_patches.json",
        help="File to store per-benchmark results",
    )
    args = parser.parse_args()

    bench_root = REPO_ROOT / args.bench_root
    results_path = Path(args.output)
    if results_path.exists():
        try:
            with results_path.open(encoding="utf-8") as f:
                results = json.load(f)
        except Exception:
            results = {}
    else:
        results = {}

    for bench_dir in collect_benchmarks(args.start, args.end, bench_root):
        name = bench_dir.name
        if name in results:
            continue
        uses_vec, data = benchmark_has_vectorization(bench_dir, args.model)
        results[name] = {
            "is_local_optimisation": bool(data and data.get("is_local_optimisation")),
            "families": data.get("families") if isinstance(data, dict) else None,
            "uses_vectorization": uses_vec,
        }
        write_text(results_path, json.dumps(results, indent=2) + "\n")

    matches = [n for n, d in results.items() if d.get("uses_vectorization")]
    for name in sorted(matches):
        print(name)


if __name__ == "__main__":
    main()
