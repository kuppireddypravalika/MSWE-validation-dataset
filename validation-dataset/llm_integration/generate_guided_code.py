#!/usr/bin/env python3
"""Generate optimized code with an analysis guidance step.

This script mirrors ``generate_optimized_code.py`` but first asks the LLM
clarifying analysis questions before requesting optimized code. Questions are
loaded from ``reports/code_comparisons/*/benchmark_<id>_<trial>.json`` using the
``potential_analysis_tool`` field. The answers are then appended to the
optimization prompt.
"""

import argparse
import json
import re
import ast
from pathlib import Path
import sys

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import ensure_dir, read_text, write_text


def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from LLM text output."""
    if not text or not text.strip():
        raise ValueError("empty response")

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        text = fenced.group(1)

    decoder = json.JSONDecoder()
    idx = text.find("{")
    while idx != -1:
        try:
            obj, end = decoder.raw_decode(text, idx)
            if isinstance(obj, dict) and "optimized_code" in obj:
                return obj
            idx = text.find("{", end)
            continue
        except json.JSONDecodeError:
            idx = text.find("{", idx + 1)
    raise json.JSONDecodeError(
        "No valid JSON object with 'optimized_code' found", text, 0
    )


def _collect_questions(bench_id: str) -> list[str]:
    """Return analysis questions for ``bench_id`` from report files."""
    base = REPO_ROOT / "reports" / "code_comparisons"
    questions: list[str] = []
    if not base.is_dir():
        return questions
    def _extract_question(obj) -> str | None:
        if isinstance(obj, dict):
            return str(obj.get("question")) if "question" in obj else None
        if isinstance(obj, str):
            text = obj.strip()
            try:
                parsed = json.loads(text)
            except Exception:
                try:
                    parsed = ast.literal_eval(text)
                except Exception:
                    parsed = None
            if isinstance(parsed, dict) and "question" in parsed:
                return str(parsed["question"])
            return text
        return str(obj)

    for model_dir in base.iterdir():
        if not model_dir.is_dir():
            continue
        for json_path in model_dir.glob(f"benchmark_{bench_id}_*.json"):
            try:
                data = json.loads(read_text(json_path))
            except Exception:
                continue
            q = data.get("potential_analysis_tool")
            if isinstance(q, list):
                for item in q:
                    val = _extract_question(item)
                    if val:
                        questions.append(val)
            else:
                val = _extract_question(q)
                if val:
                    questions.append(val)
    # Deduplicate while preserving order
    seen = set()
    ordered: list[str] = []
    for q in questions:
        if q not in seen:
            ordered.append(q)
            seen.add(q)
    return ordered


def _ask_questions(questions: list[str], code: str, model: str) -> list[tuple[str, str]]:
    """Ask ``questions`` about ``code`` using ``model``."""
    pairs = []
    for q in questions:
        prompt = f"{q}\n\nHere is the original C++ code:\n{code}"
        try:
            ans = call_llm(prompt, model=model)
        except ContextLengthError as exc:
            pairs.append((q, f"[Context length exceeded: {exc}]") )
            continue
        except Exception as exc:
            pairs.append((q, f"[LLM error: {exc}]") )
            continue
        pairs.append((q, ans.strip()))
    return pairs


def generate_with_analysis(bench_path: Path, model: str, num_solutions: int = 1) -> None:
    """Run analysis guided optimization for a benchmark."""
    if not bench_path.is_dir():
        print(f"Skipping {bench_path}: directory does not exist")
        return

    config_path = bench_path / "bench_config.json"
    function_signature = ""
    if config_path.exists():
        try:
            config = json.loads(read_text(config_path))
            function_signature = config.get("target_function_signature", "")
        except Exception:
            function_signature = ""

    prompt_file = Path(__file__).resolve().parent / "prompts" / f"{model}.json"
    if prompt_file.exists():
        prompt_data = json.loads(read_text(prompt_file))
    else:
        prompt_data = {
            "prompt": "Optimize the following C++ code:\n\n<original_code>",
            "model": model,
        }

    model_name = prompt_data.get("model", model)
    out_dir = Path("llm_outputs") / "guided_optimized_code" / model_name / bench_path.name
    ensure_dir(out_dir)

    code_files = [
        p
        for p in bench_path.iterdir()
        if p.suffix in {".cpp", ".cc", ".c", ".hpp", ".h"}
        and p.name not in {"optimized.cpp", "harness.cpp"}
    ]
    parts = [f"// {p.name}\n{read_text(p)}" for p in sorted(code_files)]
    combined_code = "\n\n".join(parts)

    bench_id_match = re.match(r"benchmark_(\d+)", bench_path.name)
    bench_id = bench_id_match.group(1) if bench_id_match else bench_path.name
    questions = _collect_questions(bench_id)

    qa_pairs = _ask_questions(questions, combined_code, model_name) if questions else []

    qa_text = "\n\n".join(f"Q: {q}\nA: {a}" for q, a in qa_pairs)

    base_prompt = prompt_data["prompt"].replace(
        "<original_code>", combined_code + ("\n\n" + qa_text if qa_text else "")
    )
    if "<function_signature>" in base_prompt:
        base_prompt = base_prompt.replace("<function_signature>", function_signature)

    qa_file = out_dir / "analysis_qna.json"
    if qa_pairs:
        write_text(qa_file, json.dumps(qa_pairs, indent=2) + "\n")

    existing = 0
    for i in range(1, num_solutions + 1):
        if (out_dir / f"optimized_{i}.cpp").exists():
            existing += 1

    remaining = num_solutions - existing
    if remaining <= 0:
        print(
            f"Requested {num_solutions} solution(s) for {bench_path} but all already exist"
        )
        return

    generated = 0
    for i in range(1, num_solutions + 1):
        out_file = out_dir / f"optimized_{i}.cpp"
        if out_file.exists():
            print(f"Skipping {out_file}: file already exists")
            continue
        try:
            response = call_llm(base_prompt, model=model_name)
        except ContextLengthError as exc:
            note_file = out_dir / f"context_length_error_{i}.txt"
            write_text(note_file, str(exc))
            print(
                f"Skipping optimized version {i} for {bench_path}: context length exceeded"
            )
            continue
        except Exception as exc:
            note_file = out_dir / f"llm_error_{i}.txt"
            write_text(note_file, str(exc))
            print(
                f"Skipping optimized version {i} for {bench_path}: failed to call LLM"
            )
            continue

        try:
            result = _parse_json_response(response)
            optimized = result["optimized_code"]
        except Exception as exc:
            note_file = out_dir / f"parse_error_{i}.txt"
            write_text(note_file, f"{exc}\n\n{response}")
            print(
                f"Skipping optimized version {i} for {bench_path}: could not parse response"
            )
            continue

        write_text(out_file, optimized)
        print(f"Saved optimized code to {out_file}")
        generated += 1
        if generated >= remaining:
            break


def main():
    parser = argparse.ArgumentParser(description="Generate guided optimized code via LLM")
    parser.add_argument("bench_dir", nargs="?", help="Path to benchmark directory")
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    parser.add_argument("--benchmarks-file", help="JSON file listing benchmarks to process")
    parser.add_argument(
        "--num-solutions", type=int, default=1, help="Number of optimized solutions to generate"
    )
    args = parser.parse_args()

    if args.benchmarks_file:
        with open(args.benchmarks_file, "r", encoding="utf-8") as f:
            data = json.load(f)
        benches = []
        for item in data.get("benchmarks", []):
            name = item.get("name") if isinstance(item, dict) else item
            if name:
                benches.append(name)
        for bench in benches:
            bench_path = Path("benchmarks") / bench
            generate_with_analysis(bench_path, args.model, args.num_solutions)
    else:
        if not args.bench_dir:
            parser.error("bench_dir is required if --benchmarks-file is not provided")
        generate_with_analysis(Path(args.bench_dir), args.model, args.num_solutions)


if __name__ == "__main__":
    main()
