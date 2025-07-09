#!/usr/bin/env python3
import os
import sys
import subprocess
import json
from pathlib import Path
from validate import compile_and_run, run_executable, mean, std

MODEL_NAME = "gpt-4o"  # Change if you're using a different model

def compile_extra(bench_dir, cpp_name, exe_name):
    """Compile a given C++ file in the benchmark directory."""
    exe_path = os.path.join(bench_dir, exe_name)
    cmd = [
        "g++", "harness.cpp", cpp_name, "-o", exe_name,
        f"-fopt-info-all=compiler_{exe_name}.log", "-std=c++17", "-O3", "-fopenmp"
    ]
    subprocess.check_call(cmd, cwd=bench_dir)
    return exe_path

def measure(name, exe_path):
    """Measure correctness and performance of a given executable."""
    correct_output = run_executable([exe_path, "--mode=correct"])
    perf_vals = run_executable([exe_path, "--mode=perf"] * 10)
    avg = mean(perf_vals)
    sd = std(perf_vals)
    return {
        "correctness_output": correct_output,
        "mean_ms": round(avg, 2),
        "std_ms": round(sd, 2)
    }

def main():
    if len(sys.argv) != 2:
        print("Usage: evaluate_agent_vs_human.py <benchmark_dir>")
        sys.exit(1)

    bench_dir = sys.argv[1]

    agent_cpp_path = os.path.join(bench_dir, "agent_optimized.cpp")
    if not os.path.exists(agent_cpp_path):
        print(f"❌ Missing agent_optimized.cpp in {bench_dir}")
        sys.exit(1)

    print(f"\n⚙️ Evaluating benchmark: {bench_dir}\n")

    # Compile original and human-optimized versions
    executables, _ = compile_and_run(bench_dir)

    # Compile agent-optimized version
    agent_exe = compile_extra(bench_dir, "agent_optimized.cpp", "bench_agent")

    # Measure all 3
    results = {
        "benchmark": bench_dir,
        "original": measure("Original", executables["original"]),
        "human_optimized": measure("Human Optimized", executables["optimized"]),
        "agent_optimized": measure("Agent Optimized", agent_exe)
    }

    # Print summary
    print("\n📊 Performance Summary:")
    print(f"Original        : {results['original']['mean_ms']} ms")
    print(f"Human Optimized : {results['human_optimized']['mean_ms']} ms")
    print(f"Agent Optimized : {results['agent_optimized']['mean_ms']} ms")

    # Save to standardized location
    bench_id = os.path.basename(bench_dir)
    out_path = Path(f"agent_outputs/{MODEL_NAME}/{bench_id}/evaluation.json")
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w") as f:
        json.dump(results, f, indent=2)

    print(f"\n✅ Saved evaluation results to: {out_path}")

if __name__ == "__main__":
    main()
