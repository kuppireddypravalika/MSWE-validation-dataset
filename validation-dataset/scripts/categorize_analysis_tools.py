#!/usr/bin/env python3
"""Categorize potential analysis tools mentioned in benchmark reports."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys

# Ensure repository root is on the Python path
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import ensure_dir

DEFAULT_TOOL_CATEGORIES: dict[str, list[str]] = {
    "Profiling": [
        "Execution Time Profiling",
        "Memory Usage Profiling",
        "Instruction-Level Profiling",
        "Hot Path Analysis",
        "I/O Profiling",
        "Thread and Concurrency Profiling",
        "Energy Consumption Profiling",
        "Function Call Profiling",
        "Value Profiling",
        "Branch Prediction Profiling",
    ],
    "Static Analysis": [
        "Value Numbering",
        "Vectorization Analysis",
        "Variable Lifetime Analysis",
        "Dead Code Analysis",
        "Control Flow Analysis",
        "Data Flow Analysis",
        "Alias Analysis",
        "Dependency Analysis",
        "Code Complexity Analysis",
        "Constant Propagation Analysis",
        "Type Checking Analysis",
    ],
    "Memory Analysis": [
        "Memory Allocation Tracking",
        "Memory Access Pattern Analysis",
        "Cache Miss Profiling",
        "Leak Detection",
        "Memory Fragmentation Analysis",
        "Buffer Overflow Detection",
        "Garbage Collection Analysis",
        "Dangling Pointer Detection",
    ],
    "Dynamic Analysis": [
        "Race Condition Detection",
        "Runtime Error Detection",
        "Exception Handling Analysis",
        "Assertion Checking",
        "Performance Bottleneck Detection",
        "Resource Usage Monitoring",
        "Dynamic Slicing",
        "Execution Path Analysis",
    ],
    "Tools": [
        "Valgrind/Cachegrind",
        "gprof/perf",
        "Intel VTune",
        "LLVM Sanitizers (ASan, MSan, UBSan)",
        "GNU Compiler Analysis Tools",
        "Visual Studio Performance Profiler",
        "Pin Tool",
        "DynInst",
    ],
}


def load_categories(path: Path) -> dict[str, list[str]]:
    """Load categories from ``path`` or return :data:`DEFAULT_TOOL_CATEGORIES`."""
    if path.exists():
        try:
            with path.open(encoding="utf-8") as f:
                data = json.load(f)
            if isinstance(data, dict):
                return {k: list(v) for k, v in data.items() if isinstance(v, list)}
        except Exception:
            pass
    return DEFAULT_TOOL_CATEGORIES.copy()


def save_categories(path: Path, categories: dict[str, list[str]]) -> None:
    """Write ``categories`` to ``path`` as JSON."""
    ensure_dir(path.parent)
    with path.open("w", encoding="utf-8") as f:
        json.dump(categories, f, indent=2)


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


def build_prompt(tool: str, categories: dict[str, list[str]]) -> str:
    """Return a categorization prompt for ``tool``."""
    cats = json.dumps(categories, indent=2)
    return (
        "Classify the following analysis technique into one of the provided "
        "categories and subcategories. If none apply, propose a new category "
        "and subcategory.\n\n"
        f"Analysis technique: {tool}\n\n"
        f"Categories: {cats}\n\n"
        "Return only a JSON object like:\n"
        "{\n  \"category\": \"<main category>\",\n"
        "  \"subcategory\": \"<subcategory>\"\n}"
    )


def categorize(tool: str, model: str, categories: dict[str, list[str]]) -> tuple[str, str]:
    """Return (category, subcategory) for ``tool``."""
    prompt = build_prompt(tool, categories)
    response = call_llm(prompt, model=model)
    data = _parse_json_response(response)
    return data.get("category", ""), data.get("subcategory", "")


def collect_analysis_tools(report_dir: Path) -> set[str]:
    """Return a set of analysis tool strings from JSON reports."""
    tools: set[str] = set()
    for path in report_dir.glob("benchmark*_*.json"):
        with path.open(encoding="utf-8") as f:
            data = json.load(f)
        val = data.get("potential_analysis_technique")
        if val is None:
            val = data.get("potential_analysis_tool")
        if isinstance(val, str):
            if val.strip():
                tools.add(val.strip())
        elif isinstance(val, list):
            for item in val:
                if isinstance(item, str) and item.strip():
                    tools.add(item.strip())
        elif isinstance(val, dict):
            for item in val.values():
                if isinstance(item, str) and item.strip():
                    tools.add(item.strip())
    return tools


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Categorize analysis tools from comparison reports"
    )
    parser.add_argument(
        "--reports-dir",
        default="reports/code_comparisons/gpt-4o-mini",
        help="Directory with benchmark comparison JSON files",
    )
    parser.add_argument(
        "--output",
        default="reports/analysis_tool_category_mapping.json",
        help="File to write mapping JSON",
    )
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    parser.add_argument(
        "--categories-file",
        default="reports/analysis_tool_categories.json",
        help="JSON file to load and store category definitions",
    )
    args = parser.parse_args()

    out_path = Path(args.output)
    ensure_dir(out_path.parent)
    progress_path = out_path.with_suffix(".progress")

    categories_path = Path(args.categories_file)
    categories = load_categories(categories_path)

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
    tools = sorted(collect_analysis_tools(report_dir))

    for idx, tool in enumerate(tools):
        if idx < processed_idx or tool in mapping:
            continue
        try:
            cat, sub = categorize(tool, args.model, categories)
        except ContextLengthError:
            print(f"Skipping due to context length: {tool}")
            processed_idx = idx + 1
            progress_path.write_text(str(processed_idx))
            continue
        except Exception as exc:
            print(f"Failed to categorize '{tool}': {exc}")
            processed_idx = idx + 1
            progress_path.write_text(str(processed_idx))
            continue

        mapping[tool] = {"category": cat, "subcategory": sub}
        if cat:
            if cat not in categories:
                categories[cat] = []
            if sub and sub not in categories[cat]:
                categories[cat].append(sub)
            save_categories(categories_path, categories)
        with out_path.open("w", encoding="utf-8") as f:
            json.dump(mapping, f, indent=2)
        processed_idx = idx + 1
        progress_path.write_text(str(processed_idx))


if __name__ == "__main__":
    main()
