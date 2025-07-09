# Benchmark 274: stable_sort for nearly sorted indexes

This benchmark is based on [commit 836af6b](https://github.com/llvm/llvm-project/commit/836af6bb36a0a71c4566ad5f5672a899581ae597) in LLDB which updated a comment explaining why `std::stable_sort` is used instead of `std::sort` when ordering symbol indexes. The code sorts vectors that are typically already almost sorted, and using `std::stable_sort` offers better performance for this case.

- **Commit:** 836af6bb36a0a71c4566ad5f5672a899581ae597
- **Author:** Owen Anderson
- **Optimization:** Use `std::stable_sort` rather than `std::sort` for vectors that are close to sorted.

## Benchmark Goal

Demonstrate the runtime difference between `std::sort` and `std::stable_sort` when sorting an index vector that is mostly sorted.
