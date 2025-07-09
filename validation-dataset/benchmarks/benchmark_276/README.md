# Benchmark 276: Polly canonicalization pass ordering

This benchmark originates from [commit 1ff6bbf](https://github.com/kavon/atJIT/commit/1ff6bbfa55ad422a000bb8ec28783720d812942f) which inserted a call to Polly's canonicalization passes before registering the main Polly optimization passes. Without this step, the optimizer received IR that was not properly canonicalized, preventing Polly from applying its polyhedral optimizations.

Our simplified model mimics a pass manager that optionally performs a canonicalization step before running a Polly-like pass. When canonicalization is skipped, the Polly pass fails to optimize the loop-heavy workload and the program executes a much larger number of iterations. When canonicalization precedes Polly, the loop is optimized and runtime is reduced.

The goal is to evaluate whether Polly's canonicalization stage dramatically impacts optimization effectiveness when added before the main passes.
