# Tutorial: Preparing Benchmarks from Performance Optimizing Commits

This tutorial provides step-by-step guidance on how to effectively extract original and optimized code from provided performance-related commits, obtain full function definitions from repositories, handle dependencies gracefully, and write corresponding test harnesses. ChatGPT may be used to assist throughout these steps.

## Step 1: Review Commit Diff

- Open the provided commit page to see changes.
- Clearly identify the specific lines modified for performance improvements.

## Step 2: Extracting Original and Optimized Code

### Original Code

- Identify the removed lines (marked with "-").
- Navigate to the commit's parent commit to view the original function.
- Extract the entire original function definition to preserve context.
- Check carefully for any dependencies such as global variables, macros, or function calls.
  - If such dependencies exist, extract those definitions as well, ensuring they are included in your benchmark in a minimal, compilable form.

### Optimized Code

- Identify the added lines (marked with "+").
- View the optimized function at the specific commit.
- Extract the complete optimized function definition to ensure all necessary context is maintained.
- Similarly handle and include all necessary dependencies and global references to maintain authenticity and ensure compilability.

### Handling Dependencies

- Include any required global variables, macros, helper functions, or external dependencies directly within the test files.
- Clearly document any modifications made to dependencies to achieve minimal compilable examples without significantly altering real-world code structure.

## Step 3: Writing a Test Harness

A test harness systematically checks correctness and measures performance.
Include inline comments in your harness explaining each step so future
contributors can understand the workflow.

## Common Pitfalls

- **Missing Dependencies:** Ensure all external dependencies are identified and included explicitly.

## Step 4: Validation and Documentation

- Perform multiple runs to ensure correctness and reproducibility.
- Clearly document:
  - Original commit source and files.
  - Description and rationale of the optimization.
- Measured performance differences.

### How Validation Works

- Each benchmark can be validated automatically using `evaluation_scripts/validate.py`.
- Running `./run_benchmarks.sh` (or `docker run --rm -v "$PWD:/workspace" perf-bench`) compiles every benchmark and invokes this script.
- For each benchmark, the script:
  1. Reads `bench_config.json` and compiles `original.cpp`, `optimized.cpp`, and `harness.cpp` with the specified flags.
  2. Executes the harness with `--mode=correct` to ensure the original and optimized implementations produce identical output.
  3. Measures performance using `--mode=perf`, verifying that the original runs for at least **5 seconds** and the optimized version is at least **1% faster**.
- If any of these checks fail, the validation exits with an error so the benchmark can be fixed before submission.

## Step 5: Submit a Pull Request

- Commit your benchmark to a fork of this repository and open a pull request against `main`.
- All pull requests trigger the **Benchmark Suite CI/CD** GitHub Actions workflow.
- The workflow runs `run_benchmarks.sh` on any changed benchmarks to ensure they pass validation.
- Make sure the workflow succeeds; PRs that fail the automated tests will not be accepted.

## Utilizing ChatGPT

You can leverage ChatGPT to extract original and optimized code for benchmarking if you encounter difficulties. Use the following prompt format to request assistance:

---

**ChatGPT Prompt Example:**

"I am developing a coding benchmark based on real-world performance optimization commits. My goal is to create a benchmark that accurately captures the original performance issue and the developer-provided optimized solution. Here is the commit diff [include diff link or paste commit diff]. Additionally, here is the original function definition [include original function definition]. Could you help me extract clearly defined original, optimized functions and a harness, preserving all necessary dependencies and the real-world context as closely as possible, to create a compilable benchmark?"

---

## Using the LLM Integration

The repository provides helper scripts to interact with an external LLM for
generating optimized code and textual explanations.

1. Set the `OPENAI_API_KEY` environment variable so the scripts can authenticate
   with the API.
2. Call `generate_optimized_code.py` with the path to your benchmark directory
   to request an LLM-produced `optimized.cpp`:

   ```bash
   python llm_integration/generate_optimized_code.py benchmarks/benchmark_001
   ```

3. For iterative refinement, run `multi_step_optimize.py`. Timing is averaged
   over five runs by default and the script stops after up to ten attempts.
   These values can be changed via `--runs` and `--max-tries`.

   ```bash
  python llm_integration/multi_step_optimize.py benchmarks/benchmark_001
  ```

  The best result is stored as `optimized.cpp` under
  `llm_outputs/multi_step_optimized_code/<model>/benchmark_001/`.
  Each run is logged in `attempts.json` with compile logs and timings.  The
  prompt for each step contains the full history of all prior attempts so the
  model can refine its code with more context.

4. Optionally ask the LLM to explain inefficiencies in the original code:

   ```bash
   python llm_integration/explain_inefficiencies.py benchmarks/benchmark_001
   ```

5. After generation, run `./run_benchmarks.sh benchmarks/benchmark_001` to
   validate that the LLM's output meets the same correctness and performance
   requirements as human-written solutions.
