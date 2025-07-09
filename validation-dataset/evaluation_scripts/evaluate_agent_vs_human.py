#!/usr/bin/env python3
import os
import sys
import subprocess
from validate import compile_and_run, run_executable, mean, std, compare_outputs

def compile_extra(bench_dir, cpp_name, exe_name):
    """Compile a given C++ file in the benchmark directory."""
    exe_path = os.path.join(bench_dir, exe_name)
    diag_path = os.path.join(bench_dir, f"compiler_{exe_name}.log")

    cmd = ["g++", "harness.cpp", cpp_name, "-o", exe_name,
           f"-fopt-info-all=compiler_{exe_name}.log", "-std=c++17", "-O3", "-fopenmp"]
    subprocess.check_call(cmd, cwd=bench_dir)

    return exe_path

def measure_and_print(name, exe_path):
    """Run correctness and performance tests on an executable."""
    print(f"\n-- {name}: Correctness Check --")
    correct_output = run_executable([exe_path, "--mode=correct"])
    print(f"{name} correctness output: {correct_output}")

    print(f"-- {name}: Performance --")
    perf_vals = run_executable([exe_path, "--mode=perf"] * 10)
    avg = mean(perf_vals)
    sd = std(perf_vals)
    print(f"{name} mean = {avg:.2f} ms, std = {sd:.2f} ms")
    return avg

def main():
    if len(sys.argv) != 2:
        print("Usage: evaluate_agent_vs_human.py <benchmark_dir>")
        sys.exit(1)

    bench_dir = sys.argv[1]

    if not os.path.exists(os.path.join(bench_dir, "agent_optimized.cpp")):
        print(f"❌ Missing agent_optimized.cpp in {bench_dir}")
        sys.exit(1)

    print(f"\n⚙️ Evaluating benchmark: {bench_dir}\n")

    # Compile original and human optimized versions
    executables, _ = compile_and_run(bench_dir)

    # Compile agent-optimized version
    agent_exe = compile_extra(bench_dir, "agent_optimized.cpp", "bench_agent")

    # Run evaluations
    original_avg = measure_and_print("Original", executables["original"])
    human_avg = measure_and_print("Human Optimized", executables["optimized"])
    agent_avg = measure_and_print("Agent Optimized", agent_exe)

    print("\n📊 Performance Summary:")
    print(f"Original        : {original_avg:.2f} ms")
    print(f"Human Optimized : {human_avg:.2f} ms")
    print(f"Agent Optimized : {agent_avg:.2f} ms")

if __name__ == "__main__":
    main()
