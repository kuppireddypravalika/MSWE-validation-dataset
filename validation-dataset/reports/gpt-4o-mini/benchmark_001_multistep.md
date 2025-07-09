# Multi-step Optimization Analysis

This document analyzes the iterative optimization attempts stored in
`llm_outputs/multi_step_optimized_code/gpt-4o-mini/benchmark_001`. Each step
corresponds to a single round of the multi-step optimizer. Execution times come
from `attempts.json`.

Step 0 represents the LLM's starting point. For context, the human hand‑optimized
implementation in `benchmarks/benchmark_001/optimized.cpp` runs in
approximately **72 ms** (see
`reports/gpt-4o-mini/benchmark_001.md`), while the LLM baseline is over
**6 seconds**.

| Step | Description | Execution Time (ms) | Outcome |
|-----:|-------------|--------------------|--------|
| 0 | LLM baseline (no `.eval()` on `x_max`) | 6446.4 | Reference timing |
| 1 | Introduced temp `x_exp` to avoid repeated broadcast/exp | 6177.2 | ✅ Runs, slightly faster |
| 2 | Declared `x_max_broadcasted` as `Tensor<T,1>`, causing dimension mismatch | N/A | ❌ Runtime abort |
| 3 | Added `x_centered` and `exp_x_centered` temps | 6566.6 | ✅ Runs, slower |
| 4 | Typed `x_max` explicitly and precomputed broadcast | 34.6 | ✅ Runs, suspiciously fast |
| 5 | Similar to step 1 with clearer variables | 6478.4 | ✅ Runs, close to step 0 runtime |
| 6 | Minor refactor of step 5 | 6582.6 | ✅ Runs, slower |
| 7 | Reverts to step 4 style | 6506.0 | ✅ Runs, roughly same as step 0 |
| 8 | Reverts to step 1 style | 6528.4 | ✅ Runs, slower |
| 9 | Explicit broadcast variable `x_max_broadcast` | 6315.6 | ✅ Runs, best valid time |
| 10 | Small refactor of step 5 | 6467.2 | ✅ Runs, close to step 0 runtime |

## Observations
- **Step 1** stored the broadcasted maximum and reused an exponentiated tensor, yielding a small speedup but still far from the hand‑optimized performance.
- **Step 2** compiled but failed at runtime due to a dimension mismatch. The LLM tried to declare the broadcasted maximum as a 1‑D tensor, likely aiming to reduce memory, but the shape was incorrect.
- **Step 3** introduced additional temporaries for clarity, which slightly slowed execution.
- **Step 4** reproduced the structure of step 0 almost exactly. The extremely low timing (34 ms) is suspect and not reproducible, so the optimizer did not treat it as a meaningful improvement.
- **Steps 5–8** experimented with variable names and ordering. These variations hovered around the step 0 runtime without approaching the hand‑optimized baseline.
- **Step 9** explicitly named the broadcast tensor and achieved the best valid runtime, though still orders of magnitude slower than the human baseline.
- **Step 10** was a minor refactor of earlier ideas with no measurable gain.

Despite iterating through ten attempts, the LLM never introduced the crucial `.eval()` used by the hand‑optimized code, so none of the generated steps matched the 72 ms baseline.

Overall, the LLM experimented with tensor broadcasting and temporary variables. The significant anomaly at step 4 suggests either a benchmark issue or a latent bug leading to an unrealistically short execution time.

## LLM Rationale and Examples

Each step in `llm_outputs/multi_step_optimized_code/gpt-4o-mini/benchmark_001`
includes the prompt and the model's JSON response. These responses record the
intent behind the code changes. Below are excerpts summarizing the LLM's
reasoning:

- **Step 1** – "Reshape and broadcast only once" and store the exponentiated
  tensor in `x_exp` to reuse the value.
- **Step 2** – "Use a temporary tensor to avoid reshaping and broadcasting
  multiple times." This version declared a `Tensor<T,1>` for the broadcasted
  maximum but crashed due to shape mismatch.
- **Step 3** – Introduced `x_centered` and `exp_x_centered` temporaries for
  clarity. This slowed the code.
- **Step 4** – "Precompute the maximum and reshape it only once for
  efficiency." Here `Eigen::Tensor<T,1> x_max = x.maximum(reduction_dims);`
  implicitly evaluated the expression when stored, so Eigen realized the tensor
  exactly once—similar to calling `.eval()` explicitly.
- **Steps 5–10** – Continued to refactor variable names and ordering without new
  ideas. Step 9's response proposed using `x_max_broadcast` to clarify the
  broadcasted tensor, yielding the best valid runtime.

These response snippets help explain why the optimizer did not progress past the
baseline runtime: the LLM repeatedly tweaked variable usage but never proposed
adding `.eval()` as seen in the hand‑optimized implementation.
