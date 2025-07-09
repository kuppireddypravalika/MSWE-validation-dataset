# Benchmark 156: Proper deduplication with std::unique

This benchmark is derived from LLVM's poolalloc project.
The original code attempted to remove duplicate callees from a vector
using `std::sort` followed by `std::unique()` without providing the same
comparator and without erasing the returned elements.
This left duplicates in the container, causing later passes to iterate
more elements than necessary.

The patch introduces a comparator used for both `std::sort` and `std::unique`
and erases the tail of the vector. This ensures duplicates are fully removed.

- **Commit:** cca37632f061f682f818cf23969fd969f02a1ad3
- **Optimization:** Remove duplicate callee entries using correct `std::unique` usage.

## Benchmark Goal

Demonstrate the performance impact of failing to erase duplicates after
`std::unique` when subsequent processing loops depend on the container size.
