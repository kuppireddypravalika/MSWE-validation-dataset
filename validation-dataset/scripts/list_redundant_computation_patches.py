#!/usr/bin/env python3
"""Detect benchmarks whose patches eliminate redundant computation.

This utility scans benchmark directories and sends their ``*.diff``
content to an LLM using the :doc:`redundant_computation_detection.prompt`.
The LLM response indicates whether the patch removes redundant computation
and returns a two-letter code describing the type of redundancy. Results
for each benchmark are stored in a single JSON file so repeated runs only
evaluate new benchmarks.
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

CONTEXT_LINES = 50


def _read_lines(path: Path) -> list[str]:
    """Return lines from ``path`` or an empty list on error."""
    try:
        with path.open(encoding="utf-8", errors="ignore") as f:
            return f.readlines()
    except Exception:
        return []


def _snippet(path: Path, start: int, length: int) -> str:
    """Return text snippet from ``path`` around ``start``."""
    lines = _read_lines(path)
    if not lines:
        return ""
    begin = max(start - CONTEXT_LINES - 1, 0)
    end = min(start + length + CONTEXT_LINES - 1, len(lines))
    snippet = ''.join(lines[begin:end])
    return f"File: {path.name} lines {begin + 1}-{end}\n" + snippet


def _collect_before_context(diff_text: str, bench_dir: Path) -> str:
    """Return snippets from ``*.before`` files referenced by ``diff_text``."""
    try:
        with (bench_dir / "files.json").open(encoding="utf-8") as f:
            mapping = json.load(f)
    except Exception:
        mapping = {}

    context_parts: list[str] = []
    current_file: Path | None = None
    for line in diff_text.splitlines():
        if line.startswith("diff --git"):
            m = re.search(r"a/(\S+) b/(\S+)", line)
            if m:
                fname = Path(m.group(2)).name
                current_file = bench_dir / f"{fname}.before"
                if not current_file.exists() and fname in mapping:
                    current_file = bench_dir / f"{Path(mapping[fname]).name}.before"
                if not current_file.exists():
                    current_file = None
        elif line.startswith("@@") and current_file is not None:
            m = re.search(r"-([0-9]+)(?:,([0-9]+))?", line)
            if m:
                start = int(m.group(1))
                length = int(m.group(2) or "1")
                snippet = _snippet(current_file, start, length)
                if snippet:
                    context_parts.append(snippet)

    return "\n".join(context_parts)

# Supported redundancy codes from the detection prompt
REDUNDANCY_CODES = {
    "RF": "repeated function calls",
    "RC": "redundant calculation",
    "LI": "loop-invariant computation",
    "DB": "repeated database/API calls",
    "UE": "unnecessary expensive recalculation",
    "CE": "duplicated conditional evaluation",
}

PROMPT_FILE = (
    Path(__file__).resolve().parents[1]
    / "llm_integration"
    / "prompts"
    / "redundant_computation_detection.prompt"
)

PROMPT_TEMPLATE = Path(PROMPT_FILE).read_text()


def build_prompt(patch_text: str) -> str:
    """Return the final LLM prompt with ``patch_text`` inserted."""
    prompt = PROMPT_TEMPLATE.replace(
        "<Diff – unified \"git show --patch\" output>", patch_text
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


def analyze_patch(patch_text: str, model: str, before_context: str = "") -> dict | None:
    """Return the LLM JSON result for ``patch_text`` or ``None`` on error."""
    combined = patch_text
    if before_context:
        combined += "\n\n" + before_context
    safe_patch = combined.replace("{", "{{").replace("}", "}}")
    prompt = build_prompt(safe_patch)
    print(f"Sending prompt to LLM:\n{prompt}\n")
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


def _normalize_redundancy_type(value: str | None) -> str | None:
    """Return a recognized redundancy code or ``None``."""
    if not isinstance(value, str):
        return None
    key = value.strip().upper()
    return key if key in REDUNDANCY_CODES else None


def patch_eliminates_redundancy(
    patch_text: str, model: str, before_context: str = ""
) -> tuple[bool, dict | None]:
    """Return ``(True, data)`` if patch removes redundant computation.

    ``data`` will include ``reason`` and ``redundancy_type`` if available.
    """
    data = analyze_patch(patch_text, model, before_context)
    if not data:
        return False, data
    code = _normalize_redundancy_type(data.get("redundancy_type"))
    if code:
        data["redundancy_type"] = code
    return bool(data.get("eliminates_redundant_computation")), data


def benchmark_eliminates_redundancy(bench_dir: Path, model: str) -> tuple[bool, dict | None]:
    """Return ``(True, data)`` if benchmark patch removes redundant computation.

    ``data`` mirrors the response from :func:`patch_eliminates_redundancy`.
    """
    patch_parts = []
    context_parts = []
    for patch_file in bench_dir.glob("*.diff"):
        try:
            text = read_text(patch_file)
        except Exception:
            continue
        patch_parts.append(text)
        context_parts.append(_collect_before_context(text, bench_dir))
    if not patch_parts:
        return False, None
    patch_text = "\n\n".join(patch_parts)
    before_ctx = "\n".join(p for p in context_parts if p)
    return patch_eliminates_redundancy(patch_text, model, before_ctx)


def _bench_name(i: int, prefix: str) -> str:
    """Return a benchmark folder name for ``i`` with the given ``prefix``.

    The repository stores directories with zero-padded numbers for values below
    100 and uses plain numbers otherwise. This helper mirrors that behaviour so
    ``_bench_name(1, "dataset")`` returns ``dataset_001`` while
    ``_bench_name(123, "dataset")`` yields ``dataset_123``.
    """

    return f"{prefix}_{i:03d}" if i < 100 else f"{prefix}_{i}"


def collect_benchmarks(start: int, end: int, bench_root: Path) -> list[Path]:
    """Return benchmark directories within ``bench_root`` for the given range."""

    benches: list[Path] = []
    for i in range(start, end + 1):
        # check dataset_* first because most directories follow this pattern
        # and fall back to benchmark_* naming. Keep the order deterministic.
        candidate_names = [
            _bench_name(i, "dataset"),
            f"dataset_{i}",
            _bench_name(i, "benchmark"),
            f"benchmark_{i}",
        ]
        for name in candidate_names:
            p = bench_root / name
            if p.is_dir():
                benches.append(p)
                break
    return benches


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List benchmarks whose patches remove redundant computation"
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
        default="reports/redundant_computation_patches.json",
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
        eliminates, data = benchmark_eliminates_redundancy(bench_dir, args.model)
        results[name] = {
            "eliminates_redundant_computation": bool(eliminates),
            "reason": data.get("reason") if isinstance(data, dict) else None,
            "redundancy_type": (
                _normalize_redundancy_type(data.get("redundancy_type"))
                if isinstance(data, dict)
                else None
            ),
        }
        write_text(results_path, json.dumps(results, indent=2) + "\n")

    matches = [n for n, d in results.items() if d.get("eliminates_redundant_computation")]
    for name in sorted(matches):
        print(name)


if __name__ == "__main__":
    main()
