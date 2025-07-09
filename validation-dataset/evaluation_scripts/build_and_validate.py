
#!/usr/bin/env python3
import os, glob, sys, json
from validate import compile_and_run, compare_outputs, parse_all_optimization_logs

# High-level wrapper used in CI to compile and validate one or more benchmarks.
# It prints progress for each benchmark and writes out any detected compiler
# optimizations for debugging purposes.

def main():
    """Entry point used by the GitHub Actions workflow."""
    import argparse

    parser = argparse.ArgumentParser(description="Compile and validate benchmarks.")
    parser.add_argument("benchmarks", nargs="*", help="Specific benchmarks to run")
    args = parser.parse_args()

    print("⚙️ Running benchmarks in default environment.")

    # Determine which benchmarks to run
    if args.benchmarks:
        bench_dirs = args.benchmarks
    else:
        bench_dirs = sorted(
            glob.glob("benchmarks/benchmark_*") +
            glob.glob("benchmarks/dataset_*")
        )

    for bench_dir in bench_dirs:
        config_path = os.path.join(bench_dir, "bench_config.json")
        if not os.path.exists(config_path):
            print(f"\n⚠️ Skipping {bench_dir}: missing bench_config.json")
            continue

        print("\n=== {} ===".format(bench_dir))
        executables, diag_files = compile_and_run(bench_dir)

        new_opts = []
        disabled_opts = []

        diag_orig = os.path.join(bench_dir, diag_files.get("original", ""))
        diag_opt = os.path.join(bench_dir, diag_files.get("optimized", ""))
        if os.path.exists(diag_orig) and os.path.exists(diag_opt):
            with open(diag_orig, "r") as f:
                log_orig = f.read()
            with open(diag_opt, "r") as f:
                log_opt = f.read()

            orig_opts = parse_all_optimization_logs(log_orig)
            opt_opts = parse_all_optimization_logs(log_opt)

            new_opts = [k for k in opt_opts if opt_opts[k] and not orig_opts.get(k, False)]
            disabled_opts = [k for k in orig_opts if orig_opts[k] and not opt_opts.get(k, False)]

            optimizations = {
                "original": orig_opts,
                "optimized": opt_opts,
                "new": new_opts,
                "disabled": disabled_opts,
            }
            with open(os.path.join(bench_dir, "optimizations_detected.json"), "w") as f_json:
                json.dump(optimizations, f_json, indent=2)

        compare_outputs(bench_dir, executables, new_opts=new_opts, disabled_opts=disabled_opts)

if __name__ == "__main__":
    main()
