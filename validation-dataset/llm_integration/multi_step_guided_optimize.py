#!/usr/bin/env python3
"""Run multi-step optimization optionally using analysis_qna guidance.

This script loads benchmark names from ``vector.json`` (or a specified file)
and iteratively optimizes each benchmark using ``multi_step_optimize``.
If an ``analysis_qna.json`` file exists for a benchmark, its Q&A pairs are
appended to the prompt so the LLM can leverage prior analysis. Results are
written under ``llm_outputs/<output_root>/<model>/<benchmark>/`` where
``output_root`` defaults to ``guided_vectorized_multi_step``.
"""

import argparse
import json
from pathlib import Path

from llm_integration import multi_step_optimize
from scripts.utilities import read_text


def _load_qna(bench: str, model: str) -> str:
    """Return formatted Q&A text for ``bench`` and ``model`` if available."""
    qna_path = Path("llm_outputs") / "guided_optimized_code" / model / bench / "analysis_qna.json"
    if not qna_path.exists():
        return ""
    try:
        pairs = json.loads(read_text(qna_path))
    except Exception:
        return ""
    lines = []
    for pair in pairs if isinstance(pairs, list) else []:
        if isinstance(pair, list) and len(pair) == 2:
            q, a = pair
            lines.append(f"Q: {q}\nA: {a}")
    return "\n\n".join(lines)


def _load_context_files(bench_dir: Path) -> str:
    """Return concatenated contents of harness and extra sources for context."""
    config_path = bench_dir / "bench_config.json"
    files: list[Path] = []
    if config_path.exists():
        try:
            cfg = json.loads(read_text(config_path))
            for src in cfg.get("sources", []):
                if src not in ("original.cpp", "optimized.cpp"):
                    path = bench_dir / src
                    if path.exists():
                        files.append(path)
        except Exception:
            pass
    harness = bench_dir / "harness.cpp"
    if harness.exists() and harness not in files:
        files.append(harness)
    for ext in ("*.h", "*.hpp"):
        for path in bench_dir.glob(ext):
            if path not in files:
                files.append(path)
    parts = []
    for p in files:
        try:
            parts.append(f"// {p.name}\n{read_text(p)}")
        except Exception:
            continue
    return "\n\n".join(parts)


def _load_benchmarks(path: Path) -> list[str]:
    with path.open() as f:
        data = json.load(f)
    benches = []
    for item in data.get("benchmarks", []):
        name = item.get("name") if isinstance(item, dict) else item
        if name:
            benches.append(name)
    return benches


def main() -> None:
    parser = argparse.ArgumentParser(
        description=(
            "Run multi-step optimization optionally using analysis_qna guidance"
        )
    )
    parser.add_argument(
        "--benchmarks-file", default="vector.json", help="JSON file listing benchmarks"
    )
    parser.add_argument("--model", default=multi_step_optimize.DEFAULT_MODEL, help="LLM model name")
    parser.add_argument("--max-tries", type=int, default=10, help="Maximum refinement iterations")
    parser.add_argument("--runs", type=int, default=5, help="Runs for timing measurement")
    parser.add_argument("--perf", action="store_true", help="Collect perf data each iteration")
    parser.add_argument(
        "--output-root",
        default="guided_vectorized_multi_step",
        help="Directory under llm_outputs for results",
    )
    parser.add_argument(
        "--hints",
        default="",
        help="Additional text appended to the optimization prompt",
    )
    parser.add_argument(
        "--no-qna",
        dest="use_qna",
        action="store_false",
        help="Disable analysis_qna guidance",
    )
    parser.set_defaults(use_qna=True)
    args = parser.parse_args()

    benches = _load_benchmarks(Path(args.benchmarks_file))
    base_template = multi_step_optimize.PROMPT_TEMPLATE

    for bench in benches:
        bench_dir = Path("benchmarks") / bench
        if not (bench_dir / "bench_config.json").exists():
            print(f"⚠️  Skipping {bench}: missing bench_config.json")
            continue
        qa_text = _load_qna(bench, args.model) if args.use_qna else ""
        context_text = _load_context_files(bench_dir)
        note = (
            "The following files are provided for context. "
            "Do NOT modify them. Only update code in original.cpp."
        )
        extras = "\n\n".join(
            part for part in [note, context_text, qa_text, args.hints] if part
        )
        if extras:
            multi_step_optimize.PROMPT_TEMPLATE = base_template.replace(
                "<original_code>", "<original_code>\n\n" + extras
            )
        else:
            multi_step_optimize.PROMPT_TEMPLATE = base_template
        multi_step_optimize.optimize_benchmark(
            bench_dir,
            args.model,
            args.max_tries,
            args.runs,
            args.perf,
            args.output_root,
        )

    multi_step_optimize.PROMPT_TEMPLATE = base_template


if __name__ == "__main__":
    main()
