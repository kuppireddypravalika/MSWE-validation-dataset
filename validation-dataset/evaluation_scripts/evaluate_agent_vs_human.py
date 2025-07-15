#!/usr/bin/env python3
import os
import sys
import subprocess
import json
from pathlib import Path
from validate import compile_and_run, run_executable, mean, std

MODEL_NAME = "gpt-4o"

def compile_extra(bench_dir, cpp_name, exe_name):
    """Compile a given C++ file in the benchmark directory."""
    exe_path = os.path.join(bench_dir, exe_name)
    cmd = [
        "g++", "harness.cpp", cpp_name, "-o", exe_name,
        f"-fopt-info-all=compiler_{exe_name}.log", "-std=c++17", "-O3", "-fopenmp"
    ]
    subprocess.check_call(cmd, cwd=bench_dir)
    return exe_path

def measure(exe_path):
    """Measure correctness and performance of a given executable."""
    correct_output = run_executable([exe_path, "--mode=correct"])
    perf_vals = []
    for _ in range(10):
        perf_output = run_executable([exe_path, "--mode=perf"])
        perf_vals.extend(perf_output)

    return {
        "correctness_output": correct_output,
        "mean_ms": round(mean(perf_vals), 2),
        "std_ms": round(std(perf_vals), 2)
    }

def main():
    if len(sys.argv) != 2:
        print("Usage: evaluate_agent_vs_human.py <benchmark_dir>")
        sys.exit(1)

    bench_dir = sys.argv[1]
    agent_cpp = os.path.join(bench_dir, "agent_optimized.cpp")

    if not os.path.exists(agent_cpp):
        print(f"❌ Missing agent_optimized.cpp in {bench_dir}")
        sys.exit(1)

    print(f"\n⚙️ Evaluating benchmark: {bench_dir}\n")

    # Compile original and human-optimized
    executables, _ = compile_and_run(bench_dir)
    exe_original = executables.get("original")
    exe_human = executables.get("optimized")

    # Compile agent-optimized
    exe_agent = compile_extra(bench_dir, "agent_optimized.cpp", "bench_agent")

    # Evaluate
    result = {
        "benchmark": bench_dir,
        "original": measure(exe_original),
        "human_optimized": measure(exe_human),
        "agent_optimized": measure(exe_agent)
    }

    # Summary
    print("\n📊 Performance Summary:")
    print(f"Original        : {result['original']['mean_ms']} ms")
    print(f"Human Optimized : {result['human_optimized']['mean_ms']} ms")
    print(f"Agent Optimized : {result['agent_optimized']['mean_ms']} ms")

    # Save output
    bench_id = os.path.basename(bench_dir)
    out_path = Path(f"agent_outputs/{MODEL_NAME}/{bench_id}/evaluation.json")
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(result, f, indent=2)

    print(f"\n✅ Saved evaluation results to: {out_path}")

if __name__ == "__main__":
    main()
