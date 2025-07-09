# Real-World Optimization Benchmark Suite

[![Benchmark Suite CI/CD](https://github.com/proywm/validation-dataset/actions/workflows/benchmark-ci.yml/badge.svg)](https://github.com/proywm/validation-dataset/actions/workflows/benchmark-ci.yml)

This repository provides a collection of real‑world code optimization examples, each with an original (inefficient) version and an optimized version. It includes a reproducible, automated harness to compile, validate correctness, and measure performance for all benchmarks at once using Docker.

## 📂 Repository Structure

```
benchmark_root/
├── benchmarks/
│   ├── benchmark_001/
│   │   ├── README.md            # Per‑benchmark description
│   │   ├── bench_config.json    # Compile flags and deps
│   │   ├── original.cpp         # Original (inefficient) code
│   │   ├── optimized.cpp        # Optimized (ground truth) code
│   │   └── harness.cpp          # Timing harness calling both versions
│   └── ...                      # More benchmarks follow same pattern
├── evaluation_scripts/
│   ├── build_and_validate.py    # Orchestrates build & validation for all
│   └── validate.py              # Compiles, runs binaries, compares outputs
├── llm_generated_outputs/       # (Reserved) LLM‑submitted solutions
├── Dockerfile                   # Single container with all dependencies
├── Dockerfile.eigen             # (Optional) Container with Eigen pre‑installed
├── README.md                    # This file
└── .gitignore
```

## 🚀 Quick Start

### Prerequisites

- Docker installed (version ≥ 20.10)
- (Optional) Git to clone this repo locally

### Build the Container

From the repository root:

```bash
docker build -t perf-bench .
```

This image includes:

- GCC toolchain (`build-essential`, `g++`)
- Python 3 and NumPy
- All system libraries used by any benchmark (Eigen3, Boost, etc.)

### Run All Benchmarks

```bash
docker run --rm -v "$PWD:/workspace" perf-bench
```

This will:

1. Discover every `benchmarks/benchmark_*` folder  
2. For each:
   - Read `bench_config.json`  
   - Compile `original.cpp`, `optimized.cpp`, and `harness.cpp`  
   - Run both binaries, capture their outputs, and compare element-wise  
3. Report **Validation passed** or **Validation failed** for each

---

## ➕ Adding a New Benchmark

To submit a new benchmark, please create a pull request following these steps:

1. **Fork and clone the repository**, create a new branch.
2. **Create a new folder** under `benchmarks/`, e.g.:

   ```
   benchmarks/benchmark_010/
   ```

3. **Add your files**:

   - `README.md`  
     Briefly describe the source, commit link, and optimization scenario.
   - `bench_config.json`  
     Specify:
     ```json
     {
       "sources":   ["original.cpp","optimized.cpp","harness.cpp"],
       "includes":  ["/usr/include/eigen3"],    // or other include paths
       "lib_paths": [],                         // any -L paths
       "libs":      [],                         // any -l libraries
       "cxxflags":  ["-std=c++17","-O3"]        // your compile flags
     }
     ```
   - `original.cpp`  
     Contains the unoptimized function and `main()` that prints numeric output.
   - `optimized.cpp`  
     Contains the optimized (ground truth) function with identical `main()`.
   - `harness.cpp`  
     A small timing harness that calls both functions (if separate) or can be a shared harness.

4. **Verify locally (optional)**  
   Without Docker, you can test compilation:
   ```bash
   g++ benchmarks/benchmark_010/original.cpp -I/usr/include/eigen3 -O3 -o orig_exe
   g++ benchmarks/benchmark_010/optimized.cpp -I/usr/include/eigen3 -O3 -o opt_exe
   ./orig_exe   # should print floats
   ./opt_exe    # should print floats
   ```

5. **Run the full suite or select benchmarks** 
   Back in the project root, to run *all* benchmarks:
   ```bash
   docker run --rm -v "$PWD:/workspace" perf-bench
   ```
   Or to run only specific benchmarks (use relative paths):
   ```bash
   docker run --rm -v "$PWD:/workspace" perf-bench \
    benchmarks/benchmark_001 \
    benchmarks/benchmark_005
   ```
   You’ll see, for each benchmark:
   - **Correctness Check output** (validation passed/failed)
   - **Performance Measurement** timings for original vs optimized code

   Example:

   ```bash
    === benchmarks/benchmark_001 ===
    -- Correctness Check --
    ✅ Correctness passed: outputs match within tolerance.
    -- Performance Measurement --
    Original time:  123 ms
    Optimized time:  45 ms
   ```
   Adjust the list of benchmark paths to focus on only those you’re actively working on.
---

## 📝 Notes

- All benchmarks must produce the **same numeric output** for the original and optimized versions.
- The harness **prints** whitespace‑separated floats; `validate.py` parses and compares them.
- If you depend on other system libraries (e.g., Boost, gflags), install them in the Dockerfile or add flags in `bench_config.json`.
- The helper scripts include inline comments describing each step so you can adapt them easily.

## 🤖 LLM Integration

The `llm_integration` directory contains optional helpers for generating
LLM‑optimized code and explanations of inefficiencies.

1. Set the environment variable `OPENAI_API_KEY` to authenticate with the
   OpenAI API.
2. Generate optimized code for a benchmark:

   ```bash
   python llm_integration/generate_optimized_code.py benchmarks/benchmark_001
   ```

   The resulting files `optimized_1.cpp`, `optimized_2.cpp`, ... are written under
   `llm_outputs/downstream_task_1_optimized_code/<model>/benchmark_001/`.

3. Run the multi-step optimizer to iteratively refine code. The tool
   averages timing over at least five runs and stops after at most ten
   iterations by default. You can adjust these values with
   `--runs` and `--max-tries`.

   ```bash
   python llm_integration/multi_step_optimize.py benchmarks/benchmark_001
   ```

   The best version is saved to
   `llm_outputs/multi_step_optimized_code/<model>/benchmark_001/optimized.cpp`.
   A file `attempts.json` in the same directory records each step's prompt,
   the LLM's reported reasoning, compile output and timing results.  Each
   prompt now includes the cumulative history of all previous attempts so the
   model can learn from the entire session.
4. Run the guided multi-step optimizer which appends any
   ``analysis_qna.json`` file to the prompt. Results are written under
   ``llm_outputs/guided_vectorized_multi_step/<model>/<benchmark>/``.

   ```bash
   docker run --rm -v "$PWD:/workspace" \
     --entrypoint python3 perf-bench \
     llm_integration/multi_step_guided_optimize.py \
       --benchmarks-file vector.json \
       --hints "Focus on loop vectorization"
   ```
5. Request a text explanation of inefficiencies:

   ```bash
   python llm_integration/explain_inefficiencies.py benchmarks/benchmark_001
   ```

   Explanations are stored in
   `llm_outputs/downstream_task_2_inefficiency_explanations/`.
6. Validate the generated code with the normal workflow:

   ```bash
   ./run_benchmarks.sh benchmarks/benchmark_001
   ```

7. Compare LLM optimizations with the hand-tuned code:

   ```bash
   python llm_integration/compare_versions.py --benchmarks-file existing_benchmarks.json
   ```

   Reports are saved under `reports/code_comparisons/<model>/` and the model
   name defaults to the one recorded in `reports/benchmark_performance.json`.

8. Derive patch alignment metrics for a benchmark:

   ```bash
   python llm_integration/derive_patch_metrics.py benchmarks/benchmark_438
   ```

   The script prints JSON with the `performance_optimization_relevance`,
   `contextual_dependency`, and `library_api_dependency` metrics. Ensure
   `OPENAI_API_KEY` is set before running.

   To process multiple benchmarks, provide a JSON list and metrics are written
   under `reports/patch_metrics/`:

   ```bash
   python llm_integration/derive_patch_metrics.py \
       --benchmarks-file existing_benchmarks.json
   ```

## 📊 Benchmark Experiment

The script `evaluation_scripts/benchmark_experiment.py` measures the performance of multiple LLM-generated solutions for each benchmark. It expects a directory structure such as `<solutions_root>/benchmark_002/optimized_1.cpp` and so on.

To run the experiment inside Docker for **benchmark 002**, override the
container's entrypoint so Python runs the experiment script directly:

```bash
# Build the image if it doesn't already exist
docker build -t perf-bench .

# Evaluate optimized_1.cpp – optimized_5.cpp for benchmark_002
docker run --rm -v "$PWD:/workspace" \
  --entrypoint python3 perf-bench \
  evaluation_scripts/benchmark_experiment.py \
  llm_outputs/downstream_task_1_optimized_code/gpt-4o-mini \
  benchmarks/benchmark_002 \
  -k 5 \
  --output reports/benchmark_002_experiment.json
```

The script writes timing results under `reports/` and skips any benchmarks that either lack corresponding solution files in `<solutions_root>` **or are already listed in the output JSON**. If you omit the benchmark directory argument, the script loads benchmark names from `existing_benchmarks.json`.

## 📂 Categorize Optimization Suggestions

`scripts/categorize_optimizations.py` collects optimization ideas from
`reports/code_comparisons/<model>/` and uses an LLM to assign each one to a
category and subcategory. Ensure `OPENAI_API_KEY` is set and run:

```bash
python scripts/categorize_optimizations.py \
  --reports-dir reports/code_comparisons/gpt-4o-mini \
  --output reports/optimization_category_mapping.json \
  --model gpt-4o-mini \
  --categories-file reports/optimization_categories.json
```

This writes a JSON file mapping each suggestion to `{"category": ..., "subcategory": ...}`.
The categories file tracks all known categories and is updated if the model proposes
new ones.


## 📂 List Benchmarks by Category

`scripts/list_benchmarks_by_category.py` lists benchmark directories containing
optimizations for a specific category or subcategory. It scans the comparison
reports under `reports/code_comparisons/<model>/` and uses the mapping file
`reports/optimization_category_mapping.json`.

Example:
```bash
python scripts/list_benchmarks_by_category.py \
  --category "Function Call and Inlining Optimizations" \
  --subcategory "Reduction of Function Call Overheads" \
  --flag addition
```

Use `--flag missed` to show benchmarks missing optimizations instead. The script
also accepts performance filters via `--compare-to`, `--slower-than`, or
`--faster-than`:

```bash
# Benchmarks where the LLM is ≥25% slower than the baseline implementation
python scripts/list_benchmarks_by_category.py \
  --category "Vectorization Optimizations" \
  --flag missed \
  --slower-than 0.25

# Benchmarks where the LLM is ≥20% faster than the baseline implementation
python scripts/list_benchmarks_by_category.py \
  --category "Vectorization Optimizations" \
  --flag missed \
  --faster-than 0.20

# Using the original implementation for comparison
python scripts/list_benchmarks_by_category.py \
  --category "Vectorization Optimizations" \
  --compare-to original
```

## 📈 Visualize Optimization Distributions

The scripts `scripts/optimization_distribution.py` and
`scripts/optimization_distribution_cat_subcat.py` generate bar charts or
heatmaps showing how often each optimization category appears in the comparison
reports. They read the mapping created by `categorize_optimizations.py` and by
default analyze `reports/code_comparisons/gpt-4o-mini`.
Each run writes an SVG heatmap to `reports/optimization_distribution/` with
the command-line options embedded in the filename and prints the path to the
generated file.

Both scripts accept optional performance filters:

* `--compare-to baseline|original` – choose whether to measure the LLM against
  the hand‑optimized baseline or the original code.
* `--slower-than PCT` – only include benchmarks where the LLM is at least this
  fraction **slower** than the reference implementation.
* `--faster-than PCT` – only include benchmarks where the LLM is at least this
  fraction **faster** than the reference implementation.

Examples:

```bash
# Basic category distribution for all benchmarks
python scripts/optimization_distribution.py

# Benchmarks where the LLM is ≥10% slower than the baseline implementation
python scripts/optimization_distribution.py \
  --compare-to baseline --slower-than 0.10

# Category/subcategory heatmap for cases where the LLM is 30% faster than the
# original implementation
python scripts/optimization_distribution_cat_subcat.py \
  --compare-to original --faster-than 0.30
```


