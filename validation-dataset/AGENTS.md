# Agent Instructions

This repository stores real-world optimization benchmarks. Each benchmark compares an original implementation with an optimized one and validates both correctness and performance. Follow the guidelines below when adding or modifying benchmarks.

## Repository Goals
- Provide minimal yet realistic code examples that demonstrate real performance optimizations.
- Ensure every benchmark compiles and runs automatically using the provided scripts and Docker environment.
- Measure performance improvement and fail if optimization is ineffective or workload is too small.

## Benchmark Structure
- Add benchmarks under `benchmarks/benchmark_xxx/` where `xxx` is a zero-padded index.
- Each folder must contain:
  - `README.md` – short description with commit link and optimization summary.
  - `bench_config.json` – compile flags and dependencies. Use the pattern:
    ```json
    {
      "sources":   ["original.cpp", "optimized.cpp", "harness.cpp"],
      "includes":  ["/usr/include/eigen3"],
      "lib_paths": [],
      "libs":      [],
      "cxxflags":  ["-std=c++17", "-O3"]
    }
    ```
  - `original.cpp` – unoptimized function with `main()` printing numeric results.
  - `optimized.cpp` – optimized version with identical output.
  - `harness.cpp` – runs both versions. Accepts `--mode=correct`/`--mode=perf` and `--impl=original|optimized` similar to `benchmark_001`.

## Passing Criteria
- `validate.py` builds the benchmark and checks:
  1. **Correctness** – outputs of original and optimized must match within tolerance.
  2. **Performance** – build the original with the `-O3` flag and ensure its average runtime is **≥ 5000 ms** (at least five seconds). The optimized implementation must be at least **1% faster**; otherwise the benchmark fails.
- Use `run_benchmarks.sh` or `docker run --rm -v "$PWD:/workspace" perf-bench` to run the checks locally.

## Creating Benchmarks
1. Review the optimization commit and extract the full original and optimized functions, including any required globals or helper code (see `Tutorial.md`).
2. Keep examples minimal but compilable. Document the commit source in the benchmark `README.md`.
3. Write a harness that prints the outputs for correctness mode and reports timings for performance mode.
4. Add inline comments to your benchmark code and harness so future contributors
   understand the workflow.
5. Run the validation script to ensure the benchmark passes before submitting a pull request.
6. Optionally generate LLM-proposed optimizations using the helpers in `llm_integration/`:

   ```bash
   python llm_integration/generate_optimized_code.py benchmarks/benchmark_xxx
   ```

   Review the output under `llm_outputs/` and run
   `./run_benchmarks.sh benchmarks/benchmark_xxx` to validate the LLM code.

