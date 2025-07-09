# Benchmark 121: LLVM FunctionAttrs early exit

This benchmark replicates [commit 6e042a2](https://github.com/llvm/llvm-project/commit/6e042a24c58bcc2dcffab3e1c8b66870da76a2e9) which added an early return in `runImpl` of the FunctionAttrs pass when the effective SCC contains only `optnone` functions. The change avoids running several expensive attribute inference helpers on empty SCCs.

Our microbenchmark creates a strongly connected component comprised entirely of `optnone` functions. The original implementation still executes each helper function, while the optimized version returns immediately.
