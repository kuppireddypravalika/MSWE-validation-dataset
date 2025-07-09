#!/usr/bin/env python3
"""Categorize optimization suggestions using an LLM."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

import sys

# Ensure repository root is on the Python path so we can import helper modules
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm, ContextLengthError
from scripts.aggregate_optimizations import collect_optimizations
from scripts.utilities import ensure_dir

DEFAULT_CATEGORIES: dict[str, list[str]] = {
    "Compiler Directives and Platform-Specific Optimizations": [
        "Compiler Flags",
        "Platform and Endian-Specific Optimizations",
    ],
    "Memory Management Optimizations": [
        "Pre-allocation and Reservation",
        "Efficient Allocation and Deallocation",
    ],
    "Loop and Control Flow Optimizations": [
        "Loop Unrolling and Flattening",
        "Early Exits and Condition Simplifications",
    ],
    "Computation and Mathematical Optimizations": [
        "Pre-computation and Constant Folding",
        "Arithmetic Simplifications and Bitwise Optimizations",
        "Specialized Mathematical Computations",
    ],
    "Data Structure and Access Optimizations": [
        "Cache Locality Improvements",
        "Efficient Data Structures",
        "In-place Data Manipulations",
    ],
    "Parallelization and Concurrency Optimizations": [
        "SIMD and Vectorization",
        "Multithreading and Parallel STL",
    ],
    "Atomic and Lock-Free Optimizations": [
        "Atomic Operations",
        "Lock-Free Mechanisms",
    ],
    "Function Call and Inlining Optimizations": [
        "Function Inlining",
        "Reduction of Function Call Overheads",
    ],
    "String, Parsing, and I/O Optimizations": [
        "Direct Parsing and Regex Avoidance",
        "Efficient String Manipulations",
    ],
    "Algorithmic and Sorting Optimizations": [
        "Efficient Sorting and Merging",
        "Algorithmic Improvements",
    ],
    "Robustness and Validation Improvements": [
        "Enhanced Error Handling",
        "Robustness Checks and Validations",
    ],
    "Miscellaneous Optimizations": [
        "Volatility Reduction",
        "Dynamic Adjustments and Thresholding",
    ],
}


def load_categories(path: Path) -> dict[str, list[str]]:
    """Load categories from JSON ``path`` or return :data:`DEFAULT_CATEGORIES`."""
    if path.exists():
        try:
            with path.open(encoding="utf-8") as f:
                data = json.load(f)
            if isinstance(data, dict):
                return {k: list(v) for k, v in data.items() if isinstance(v, list)}
        except Exception:
            pass
    return DEFAULT_CATEGORIES.copy()


def save_categories(path: Path, categories: dict[str, list[str]]) -> None:
    """Write ``categories`` to ``path`` as JSON."""
    ensure_dir(path.parent)
    with path.open("w", encoding="utf-8") as f:
        json.dump(categories, f, indent=2)


def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from LLM text output."""
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


def build_prompt(opt: str, categories: dict[str, list[str]]) -> str:
    """Return the categorization prompt for ``opt``."""
    cats = json.dumps(categories, indent=2)
    return (
        "Classify the following optimization suggestion into one of the provided "
        "categories and subcategories. If none apply, propose a new category and "
        "subcategory.\n\n"
        f"Optimization: {opt}\n\n"
        f"Categories: {cats}\n\n"
        "Return only a JSON object like:\n"
        "{\n  \"category\": \"<main category>\",\n"
        "  \"subcategory\": \"<subcategory>\"\n}"
    )


def categorize(opt: str, model: str, categories: dict[str, list[str]]) -> tuple[str, str]:
    """Return (category, subcategory) for ``opt``."""
    prompt = build_prompt(opt, categories)
    response = call_llm(prompt, model=model)
    data = _parse_json_response(response)
    return data.get("category", ""), data.get("subcategory", "")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Categorize optimizations from comparison reports"
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark comparison JSON files",
    )
    parser.add_argument(
        "--output",
        default="reports/optimization_category_mapping.json",
        help="File to write mapping JSON",
    )
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    parser.add_argument(
        "--categories-file",
        default="reports/optimization_categories.json",
        help="JSON file to load and store category definitions",
    )
    args = parser.parse_args()

    out_path = Path(args.output)
    ensure_dir(out_path.parent)
    progress_path = out_path.with_suffix(".progress")

    categories_path = Path(args.categories_file)
    categories = load_categories(categories_path)

    # Load any existing mapping to allow resuming
    mapping: dict[str, dict[str, str]] = {}
    if out_path.exists():
        try:
            with out_path.open(encoding="utf-8") as f:
                mapping = json.load(f)
        except Exception:
            mapping = {}

    processed_idx = 0
    if progress_path.exists():
        try:
            processed_idx = int(progress_path.read_text().strip())
        except ValueError:
            processed_idx = 0

    report_dir = Path(args.reports_dir)
    optimizations = sorted(collect_optimizations(report_dir))

    for idx, opt in enumerate(optimizations):
        if idx < processed_idx or opt in mapping:
            continue
        try:
            cat, sub = categorize(opt, args.model, categories)
        except ContextLengthError:
            print(f"Skipping due to context length: {opt}")
            processed_idx = idx + 1
            progress_path.write_text(str(processed_idx))
            continue
        except Exception as exc:
            print(f"Failed to categorize '{opt}': {exc}")
            processed_idx = idx + 1
            progress_path.write_text(str(processed_idx))
            continue

        mapping[opt] = {"category": cat, "subcategory": sub}
        if cat:
            if cat not in categories:
                categories[cat] = []
            if sub and sub not in categories[cat]:
                categories[cat].append(sub)
            # Persist newly discovered categories so subsequent iterations
            # include them in the prompt.
            save_categories(categories_path, categories)
        with out_path.open("w", encoding="utf-8") as f:
            json.dump(mapping, f, indent=2)
        processed_idx = idx + 1
        progress_path.write_text(str(processed_idx))


if __name__ == "__main__":
    main()
