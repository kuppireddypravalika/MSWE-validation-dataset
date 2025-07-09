# Benchmark 146: LLVM InlineCost heuristic change

This benchmark is derived from [LLVM commit 74a6842](https://github.com/llvm/llvm-project/commit/74a684222d45b4d9eb3986a10c92c9ec67378568).
The change removed a size based penalty when deciding whether to inline into a large caller function.
Dropping this penalty allows inlining opportunities that can reduce runtime of hot loops.

Our minimal reproduction models the inline cost computation. The `original` implementation
adds a penalty proportional to the caller's size, preventing inlining in our scenario.
The `optimized` version omits this penalty so the call site is inlined.

The workload repeatedly sums values using either an inlined or non-inlined call depending
on the computed cost. Removing the penalty enables inlining which avoids function call overhead.
