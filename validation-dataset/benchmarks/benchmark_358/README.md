# Benchmark 358: Early exit in salvageDebugInfo

This benchmark originates from [LLVM commit 9bfb90b1f8c9](https://github.com/llvm/llvm-project/commit/9bfb90b1f8c919e5c28c34b6cc87f3b1f29b94a6) by Vedant Kumar.
The patch adds a quick `isUsedByMetadata()` check to `salvageDebugInfo` so
that most calls return immediately without searching for debug users.

The microbenchmark builds a list of instructions where only a small
fraction are referenced by debug metadata. The original implementation
always scans the entire debug-use list, while the optimized version
returns early for instructions without metadata. The workload is scaled
so the runtime difference becomes measurable.
