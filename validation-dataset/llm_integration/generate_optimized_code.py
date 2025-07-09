#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
import sys
import re
from llm_api_utils import call_llm, ContextLengthError


# This utility contacts an LLM to produce one or more optimized versions of the
# provided benchmark code. The resulting files are stored under
# ``llm_outputs/``.

def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from LLM text output.

    The LLM should return a JSON object but may include extra text or
    formatting such as markdown fences. This helper extracts the first
    JSON object found and parses it.
    """
    if not text or not text.strip():
        raise ValueError("empty response")

    # Remove common markdown code fences around JSON
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

# Ensure repository root is on the Python path so we can import from scripts/
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))
from scripts.utilities import ensure_dir, read_text, write_text


def generate_for_benchmark(bench_path: Path, model: str, num_solutions: int = 1) -> None:
    """Run the optimization workflow for a single benchmark.

    ``num_solutions`` controls how many optimized variants should be produced.
    Each result is written to ``optimized_<n>.cpp`` where ``n`` starts at 1.
    """

    if not bench_path.is_dir():
        print(f"Skipping {bench_path}: directory does not exist")
        return


    # Attempt to load the function signature from bench_config.json
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

    # Determine output location. Multiple solutions will be stored as
    # ``optimized_<n>.cpp`` within this directory so downstream tools can
    # easily evaluate each variant.
    model_name = prompt_data.get("model", model)
    out_dir = Path("llm_outputs") / "downstream_task_1_optimized_code" / model_name / bench_path.name
    ensure_dir(out_dir)

    code_files = [
        p
        for p in bench_path.iterdir()
        if p.suffix in {".cpp", ".cc", ".c", ".hpp", ".h"}
        and p.name not in {"optimized.cpp", "harness.cpp"}
    ]
    parts = []
    for p in sorted(code_files):
        parts.append(f"// {p.name}\n{read_text(p)}")
    combined_code = "\n\n".join(parts)

    prompt = prompt_data["prompt"].replace("<original_code>", combined_code)
    if "<function_signature>" in prompt:
        prompt = prompt.replace("<function_signature>", function_signature)

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
            response = call_llm(prompt, model=prompt_data.get("model", model))
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
    """Command line interface for optimizing one or more benchmarks."""
    parser = argparse.ArgumentParser(description="Generate optimized code via LLM")
    parser.add_argument("bench_dir", nargs="?", help="Path to benchmark directory")
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    parser.add_argument("--benchmarks-file", help="JSON file listing benchmarks to process")
    parser.add_argument(
        "--num-solutions",
        type=int,
        default=1,
        help="Number of optimized solutions to generate",
    )
    args = parser.parse_args()

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
        for bench in benchmarks:
            bench_path = Path("benchmarks") / bench
            generate_for_benchmark(bench_path, args.model, args.num_solutions)
    else:
        if not args.bench_dir:
            parser.error("bench_dir is required if --benchmarks-file is not provided")
        generate_for_benchmark(Path(args.bench_dir), args.model, args.num_solutions)


if __name__ == "__main__":
    main()
