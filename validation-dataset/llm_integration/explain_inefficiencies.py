#!/usr/bin/env python3
import argparse
from datetime import datetime
from pathlib import Path
from llm_api_utils import call_llm, ContextLengthError
from scripts.utilities import ensure_dir, read_text, write_text

# Fetches a textual explanation of inefficiencies in the benchmark's original
# code by querying an LLM.


def main():
    """Request an inefficiency explanation for a benchmark."""
    parser = argparse.ArgumentParser(description="Explain code inefficiencies via LLM")
    parser.add_argument("bench_dir", help="Path to benchmark directory")
    parser.add_argument("--model", default="gpt-4o-mini", help="LLM model name")
    args = parser.parse_args()

    bench_path = Path(args.bench_dir)
    if not bench_path.is_dir():
        print(f"Skipping {bench_path}: directory does not exist")
        return

    code_path = bench_path / "original.cpp"
    if not code_path.exists():
        print(f"Skipping {bench_path}: original.cpp not found")
        return

    out_dir = Path("llm_outputs") / "downstream_task_2_inefficiency_explanations" / args.model / bench_path.name
    ensure_dir(out_dir)
    if any(out_dir.glob("explanation_*.txt")):
        print(f"Skipping {bench_path}: explanation already exists")
        return

    code_text = read_text(code_path)
    prompt = f"Identify inefficiencies in the following C++ code and suggest optimizations:\n\n{code_text}"

    try:
        explanation = call_llm(prompt, model=args.model)
    except ContextLengthError as exc:
        note_file = out_dir / "context_length_error.txt"
        write_text(note_file, str(exc))
        print(f"Skipping {bench_path}: context length exceeded")
        return

    timestamp = datetime.utcnow().strftime("%Y%m%d_%H%M%S")
    out_file = out_dir / f"explanation_{timestamp}.txt"
    write_text(out_file, explanation)
    print(f"Saved explanation to {out_file}")


if __name__ == "__main__":
    main()
