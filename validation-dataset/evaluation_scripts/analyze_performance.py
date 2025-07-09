#!/usr/bin/env python3
import os
import shutil
import sys
import subprocess


def run_validate(bench_dir, description):
    print(f"\n🔍 Now running benchmark: {description}")
    subprocess.run([
        "python3", "evaluation_scripts/validate.py", bench_dir
    ], check=True)


def main():
    if len(sys.argv) < 2:
        print("Usage: evaluate_agent_vs_human.py <benchmark_dir>")
        sys.exit(1)

    bench_dir = sys.argv[1]
    original_opt_path = os.path.join(bench_dir, "optimized.cpp")
    agent_opt_path = os.path.join(bench_dir, "agent_optimized.cpp")
    temp_backup_path = os.path.join(bench_dir, "optimized.cpp.bak")

    if not os.path.exists(agent_opt_path):
        print(f"❌ Missing agent_optimized.cpp in {bench_dir}. Cannot continue.")
        sys.exit(1)

    # Step 1: Run original vs human-optimized
    run_validate(bench_dir, "Original vs Human-Optimized")

    # Step 2: Backup human-optimized and replace with agent-optimized
    shutil.copy2(original_opt_path, temp_backup_path)
    shutil.copy2(agent_opt_path, original_opt_path)

    # Step 3: Run original vs agent-optimized
    run_validate(bench_dir, "Original vs Agent-Optimized")

    # Step 4: Restore human-optimized
    shutil.move(temp_backup_path, original_opt_path)
    print("\n✅ Finished benchmarking agent and human optimized code.")


if __name__ == "__main__":
    main()
