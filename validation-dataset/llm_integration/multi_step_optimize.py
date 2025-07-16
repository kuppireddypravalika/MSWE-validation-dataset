#!/usr/bin/env python3
"""Iteratively optimize benchmark code using an LLM.

This script extends `generate_optimized_code.py` by running
multiple refinement steps. After each iteration it attempts to
compile and run the produced code, then feeds any compilation
errors, runtime errors and timing results back to the model for
the next round. The process stops after ``--max-tries`` attempts
(default 10) or when a given benchmark compiles and runs
successfully.

All intermediate versions are stored under
``llm_outputs/multi_step_optimized_code/<model>/<benchmark>/``.
The best performing version is copied to ``best.cpp`` in the same
folder.
"""

import argparse
import json
import subprocess
from pathlib import Path
import statistics
import difflib
import sys
import os
import shutil
from typing import Optional, List, Dict

# Ensure repository root is on the Python path so we can import helper modules
REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT))

from llm_integration.llm_api_utils import call_llm, ContextLengthError
from evaluation_scripts.validate import compile_and_run, run_executable
# ``run_llm_outputs`` exposes ``run_benchmark`` which executes compiled
# benchmarks and returns timing data. Import this helper for timing
# measurements during optimization.
from evaluation_scripts.run_llm_outputs import run_benchmark
from scripts.utilities import ensure_dir, read_text, write_text


PROMPT_FILE = Path(__file__).resolve().parent / "prompts" / "multi_step.json"
if PROMPT_FILE.exists():
    PROMPT_DATA = json.loads(read_text(PROMPT_FILE))
else:
    PROMPT_DATA = {
        "prompt": (
            "You are an expert C++ performance engineer. Given the original code "
            "and previous attempt information, provide an improved implementation."\
            " Respond with JSON containing the key 'optimized_code'.\n\n"
            "<original_code>\n\nPrevious attempt info:\n<attempt_info>"
        ),
        "model": "gpt-4o-mini",
    }

PROMPT_TEMPLATE = PROMPT_DATA["prompt"]
DEFAULT_MODEL = PROMPT_DATA.get("model", "gpt-4o-mini")


def _parse_json_response(text: str) -> dict:
    """Return parsed JSON from LLM text output."""
    if not text or not text.strip():
        raise ValueError("empty response")

    # Remove common markdown code fences around JSON
    fenced = None
    import re

    fenced = re.search(r"```(?:json)?\s*(\{.*?\})\s*```", text, re.DOTALL)
    if fenced:
        text = fenced.group(1)

    decoder = json.JSONDecoder()
    idx = text.find("{")
    while idx != -1:
        try:
            obj, _ = decoder.raw_decode(text, idx)
            return obj
        except json.JSONDecodeError:
            idx = text.find("{", idx + 1)
    raise json.JSONDecodeError("No valid JSON object found", text, 0)


def _compile_llm_code(bench_dir: Path, llm_file: Path, exe_name: str) -> tuple[Optional[Path], str]:
    """Compile ``llm_file`` with the benchmark harness.

    Returns (executable_path or None, compiler_output).
    """
    from evaluation_scripts.validate import load_config

    config = load_config(str(bench_dir))
    harness_file = "harness.cpp"
    diag_file = f"{exe_name}_optimizations.log"
    extra_files = [
        src
        for src in config.get("sources", [])
        if src not in (harness_file, "original.cpp", "optimized.cpp")
    ]

    llm_file = llm_file.resolve()

    cmd = ["g++", harness_file, str(llm_file)] + extra_files + [
        "-o",
        exe_name,
        f"-fopt-info-all={diag_file}",
    ]
    cmd += ["-I", str(bench_dir.resolve())]
    cmd += ["-I" + inc for inc in config.get("includes", [])]
    cmd += ["-L" + lp for lp in config.get("lib_paths", [])]
    cmd += [f"-l{lib}" for lib in config.get("libs", [])]
    cmd += config.get("cxxflags", [])

    env_cxxflags = os.environ.get("CXXFLAGS")
    if env_cxxflags:
        cmd += env_cxxflags.split()

    result = subprocess.run(cmd, cwd=bench_dir, capture_output=True, text=True)
    output = result.stdout + result.stderr
    if result.returncode != 0:
        return None, output
    return bench_dir / exe_name, output


def _measure_time(
    exe: Path, runs: int, timeout: float | None = None
) -> tuple[float, float]:
    """Return (mean, std) execution time over ``runs``.

    If no valid timing data is returned, ``float('inf')`` and ``float('nan')``
    are used so callers can gracefully handle the failure.

    Parameters
    ----------
    exe : Path
        Executable to benchmark.
    runs : int
        Number of runs to execute.
    timeout : float | None
        Maximum allowed seconds per run. ``None`` disables the limit.
    """
    # ``run_llm_outputs`` provides ``run_benchmark`` which executes the binary
    # multiple times and returns a list of measured runtimes. Use this helper to
    # derive statistics for our optimization loop.
    times = run_benchmark(exe, runs, timeout=timeout)
    if not times:
        print(f"⚠️  No timing data produced for {exe}")
        return float("inf"), float("nan")
    mean_val = statistics.mean(times)
    std_val = statistics.stdev(times) if len(times) > 1 else 0.0
    return mean_val, std_val


def _collect_perf_data(exe: Path) -> str:
    """Run Linux perf and return a short textual summary.

    If the ``perf`` tool is not available, report this gracefully instead of
    raising an exception.
    """
    if shutil.which("perf") is None:
        return "perf tool not available"

    perf_data = exe.with_suffix(".perf.data")
    try:
        record = subprocess.run(
            ["perf", "record", "-o", str(perf_data), "--", str(exe)],
            capture_output=True,
            text=True,
        )
        if record.returncode != 0:
            return f"perf failed: {record.stderr.strip()}"

        report = subprocess.run(
            ["perf", "report", "--stdio", "--line", "-i", str(perf_data)],
            capture_output=True,
            text=True,
        )
        if report.returncode != 0:
            return f"perf report failed: {report.stderr.strip()}"

        lines = report.stdout.splitlines()[:20]
        return "\n".join(lines)
    except Exception as exc:
        return f"perf failed: {exc}"
    finally:
        if perf_data.exists():
            try:
                perf_data.unlink()
            except OSError:
                pass


def optimize_benchmark(
    bench_dir: Path,
    model: str,
    max_tries: int,
    runs: int,
    use_perf: bool = False,
    output_root: str = "multi_step_optimized_code",
) -> None:
    """Run the multi-step optimization workflow for one benchmark.

    ``output_root`` specifies the directory under ``llm_outputs`` where all
    intermediate and final results are written.
    """
    if not bench_dir.is_dir():
        print(f"Skipping {bench_dir}: directory does not exist")
        return

    out_dir = Path("llm_outputs") / output_root / model / bench_dir.name
    ensure_dir(out_dir)

    history_file = out_dir / "attempts.json"
    history: List[Dict] = []

    code_files = [
        p
        for p in bench_dir.iterdir()
        if p.suffix in {".cpp", ".cc", ".c", ".hpp", ".h"}
        and p.name not in {"optimized.cpp", "harness.cpp"}
    ]
    parts = []
    for p in sorted(code_files):
        parts.append(f"// {p.name}\n{read_text(p)}")
    combined_code = "\n\n".join(parts)

    try:
        executables, _ = compile_and_run(str(bench_dir))
    except FileNotFoundError:
        print(f"Skipping {bench_dir}: missing bench_config.json")
        return

    orig_exe = Path(executables["original"])
    orig_mean, orig_std = _measure_time(orig_exe, runs)
    orig_outputs = run_executable([str(orig_exe), "--mode=correct"])
    orig_perf = _collect_perf_data(orig_exe) if use_perf else ""
    record = {
        "step": 0,
        "description": "baseline",
        "exec_time_mean_ms": orig_mean,
        "exec_time_std_ms": orig_std,
    }
    if use_perf:
        perf_file = out_dir / "original_perf.txt"
        write_text(perf_file, orig_perf)
        record["perf_file"] = str(perf_file)
    history.append(record)
    write_text(history_file, json.dumps(history, indent=2))

    attempt_history: List[str] = [
        f"Original execution time: mean = {orig_mean:.2f} ms, std = {orig_std:.2f} ms"
    ]
    if orig_perf:
        attempt_history[0] += f"\nperf report:\n{orig_perf}"
    attempt_info = ""
    best_time = None
    best_file = None

    for step in range(1, max_tries + 1):
        step_attempt = "\n".join(attempt_history)
        prompt = PROMPT_TEMPLATE
        prompt = prompt.replace("<original_code>", combined_code)
        prompt = prompt.replace("<attempt_info>", step_attempt)

        prompt_file = out_dir / f"step_{step}_prompt.txt"
        write_text(prompt_file, prompt)

        try:
            response = call_llm(prompt, model=model)
        except ContextLengthError as exc:
            note_file = out_dir / f"step_{step}_context_length_error.txt"
            write_text(note_file, str(exc))
            print(f"Skipping {bench_dir}: context length exceeded")
            return

        response_file = out_dir / f"step_{step}_response.txt"
        write_text(response_file, response)

        try:
            result = _parse_json_response(response)
            optimized = result["optimized_code"]
            analysis_text = result.get("analysis", "")
        except Exception as exc:
            error_file = out_dir / f"step_{step}_parse_error.txt"
            write_text(error_file, f"{exc}\n\n{response}")
            record = {
                "step": step,
                "prompt_file": str(prompt_file),
                "response_file": str(response_file),
                "parse_error": str(exc),
            }
            history.append(record)
            write_text(history_file, json.dumps(history, indent=2))
            attempt_info = (
                f"LLM response could not be parsed: {exc}\n"
                f"Raw response:\n{response}"
            )
            attempt_history.append(attempt_info)
            print(f"Failed to parse LLM response at step {step}: {exc}")
            continue

        step_file = out_dir / f"step_{step}.cpp"
        write_text(step_file, optimized)

        diff_lines = difflib.unified_diff(
            combined_code.splitlines(),
            optimized.splitlines(),
            fromfile="original",
            tofile=f"step_{step}",
            lineterm="",
        )
        diff_text = "\n".join(diff_lines)

        exe_name = f"bench_llm_step_{step}"
        exe_path, compile_output = _compile_llm_code(bench_dir, step_file, exe_name)

        record = {
            "step": step,
            "prompt_file": str(prompt_file),
            "response_file": str(response_file),
            "code_file": str(step_file),
            "analysis": analysis_text,
            "compile_output": compile_output,
            "compile_success": bool(exe_path),
        }

        if not exe_path:
            attempt_info = (
                f"Diff from original code:\n{diff_text}\n"
                f"Compilation failed. Output:\n{compile_output}\n"
                f"LLM analysis: {analysis_text}"
            )
            attempt_history.append(attempt_info)
            history.append(record)
            write_text(history_file, json.dumps(history, indent=2))
            continue

        try:
            llm_outputs = run_executable([str(exe_path), "--mode=correct"])
            correct = (
                len(llm_outputs) == len(orig_outputs)
                and all(abs(o - l) < 1e-6 for o, l in zip(orig_outputs, llm_outputs))
            )
            record["correctness_passed"] = correct
            if not correct:
                attempt_info = (
                    f"Diff from original code:\n{diff_text}\n"
                    f"Correctness check failed.\n"
                    f"LLM analysis: {analysis_text}"
                )
                attempt_history.append(attempt_info)
                history.append(record)
                write_text(history_file, json.dumps(history, indent=2))
                continue

            timeout = orig_mean / 1000.0 + 2.0
            exec_mean, exec_std = _measure_time(exe_path, runs, timeout=timeout)
            perf_text = _collect_perf_data(exe_path) if use_perf else ""
            record.update({
                "runtime_success": exec_mean != float("inf"),
                "execution_time_mean_ms": exec_mean,
                "execution_time_std_ms": exec_std,
            })
            if exec_mean == float("inf"):
                record["timeout"] = "Due to execution takes longer than original"
                attempt_info = (
                    f"Diff from original code:\n{diff_text}\n"
                    "Execution exceeded time limit.\n"
                    f"LLM analysis: {analysis_text}"
                )
                attempt_history.append(attempt_info)
                history.append(record)
                write_text(history_file, json.dumps(history, indent=2))
                continue
            if exec_mean - orig_mean >= 5000:
                print(
                    f"Skipping {bench_dir}: optimized run slower than original by {exec_mean - orig_mean:.2f} ms"
                )
                if perf_text:
                    perf_note = f"\nperf report:\n{perf_text}"
                else:
                    perf_note = ""
                attempt_info = (
                    f"Diff from original code:\n{diff_text}\n"
                    f"Compilation succeeded. Execution time: mean = {exec_mean:.2f} ms, std = {exec_std:.2f} ms. "
                    f"Original execution time: mean = {orig_mean:.2f} ms, std = {orig_std:.2f} ms\n"
                    f"LLM analysis: {analysis_text}{perf_note}"
                )
                attempt_history.append(attempt_info)
                record["skipped"] = True
                history.append(record)
                write_text(history_file, json.dumps(history, indent=2))
                continue
            if use_perf:
                perf_file = out_dir / f"step_{step}_perf.txt"
                write_text(perf_file, perf_text)
                record["perf_file"] = str(perf_file)
            attempt_info = (
                f"Diff from original code:\n{diff_text}\n"
                f"Compilation succeeded. Execution time: mean = {exec_mean:.2f} ms, std = {exec_std:.2f} ms. "
                f"Original execution time: mean = {orig_mean:.2f} ms, std = {orig_std:.2f} ms\n"
                f"LLM analysis: {analysis_text}"
            )
            if perf_text:
                attempt_info += f"\nperf report:\n{perf_text}"
            attempt_history.append(attempt_info)
            if best_time is None or exec_mean < best_time:
                best_time = exec_mean
                best_file = step_file
        except subprocess.CalledProcessError as e:
            record.update({
                "runtime_success": False,
                "runtime_error": str(e),
                "correctness_passed": False,
            })
            attempt_info = (
                f"Diff from original code:\n{diff_text}\n"
                f"Runtime error: {e}\n"
                f"LLM analysis: {analysis_text}"
            )
            attempt_history.append(attempt_info)
            history.append(record)
            write_text(history_file, json.dumps(history, indent=2))
            continue

        history.append(record)
        write_text(history_file, json.dumps(history, indent=2))

    if best_file:
        best_source = read_text(best_file)
        write_text(out_dir / "best.cpp", best_source)
        # Also provide an ``optimized.cpp`` alias so existing evaluation
        # tooling like ``run_llm_outputs.py`` can process the result
        write_text(out_dir / "optimized.cpp", best_source)
        print(
            f"Best version from step {best_file.name} with mean time {best_time:.2f} ms "
            f"saved to {out_dir/'best.cpp'} and {out_dir/'optimized.cpp'}"
        )
    else:
        print("No successful optimized version produced")

    # Measure performance of the human optimized implementation for comparison
    human_exe = Path(executables.get("optimized"))
    human_mean, human_std = _measure_time(human_exe, runs)
    history.append({
        "step": history[-1]["step"] + 1,
        "description": "human_optimized",
        "exec_time_mean_ms": human_mean,
        "exec_time_std_ms": human_std,
    })
    write_text(history_file, json.dumps(history, indent=2))

    print(
        f"Human optimized execution time: mean = {human_mean:.2f} ms, std = {human_std:.2f} ms"
    )
    if best_time is not None:
        if best_time > 0:
            print(
                f"LLM speedup vs human optimized: {human_mean / best_time:.2f}x"
            )
        else:
            print("LLM optimized code produced zero execution time; speedup undefined")


def main():
    parser = argparse.ArgumentParser(description="Iteratively generate optimized code via LLM")
    parser.add_argument("bench_dir", help="Path to benchmark directory")
    parser.add_argument("--model", default=DEFAULT_MODEL, help="LLM model name")
    parser.add_argument("--max-tries", type=int, default=10, help="Maximum refinement iterations")
    parser.add_argument("--runs", type=int, default=5, help="Runs for timing measurement")
    parser.add_argument(
        "--perf",
        action="store_true",
        help="Collect Linux perf data for each iteration",
    )
    parser.add_argument(
        "--output-root",
        default="multi_step_optimized_code",
        help="Directory under llm_outputs for results",
    )
    args = parser.parse_args()

    optimize_benchmark(
        Path(args.bench_dir),
        args.model,
        args.max_tries,
        args.runs,
        args.perf,
        args.output_root,
    )


if __name__ == "__main__":
    main()

