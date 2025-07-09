# Benchmark 255: LLVM SmallSetVector Optimization

This benchmark is based on [LLVM commit 3d42d0a](https://github.com/llvm/llvm-project/commit/3d42d0a67ff9c417e40ad422a0f3b190d36f1712) which replaced a `SetVector` with a `SmallSetVector` in the GlobalISel legalizer pass. The change avoids the overhead of `std::set` and reduces allocations by using a small, preallocated buffer.

Our simplified benchmark models the same behaviour with custom containers. The `original` version uses a `SetVector` implemented with `std::set` plus `std::vector`. The `optimized` version uses a `SmallSetVector` backed by `std::unordered_set` with reserved space to mimic LLVM's small‑buffer optimization.
