# Benchmark 202: Reusing Builder in LLVM InstCombine

This benchmark is based on LLVM commit [ae7527a7](https://github.com/llvm/llvm-project/commit/ae7527a79e936c7e02e2deeecd2dc7b44ef2ffc1).
The patch avoids creating a new `IRBuilder` inside `foldSelectCttzCtlz` and reuses
InstCombine's existing builder instead. Reusing the builder ensures newly
inserted instructions are placed on InstCombine's worklist and saves the cost of
reconstructing the builder on each call.

The benchmark simulates repeated calls to `foldSelectCttzCtlz` where a builder is
constructed per invocation (original) versus passing in the existing builder
(optimized).
