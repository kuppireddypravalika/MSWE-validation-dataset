# Benchmark 380: Avoiding unnecessary Queue reallocations

This benchmark is based on Mozilla's TraceMonkey JIT commit [142909c](https://github.com/mozilla/gecko-dev/commit/142909c3ddb74f236e5c2f4b186d31439dba2fc0).
The patch modified `Queue::ensure` to skip reallocation when the current
capacity already satisfies the requested size and to grow the buffer exponentially.
The original implementation always reallocated, causing quadratic behaviour
when elements were appended repeatedly.

- **Commit:** [142909c](https://github.com/mozilla/gecko-dev/commit/142909c3ddb74f236e5c2f4b186d31439dba2fc0)
- **Author:** Andreas Gal
- **Optimization:** Avoid redundant reallocations in `Queue::ensure`.

## Benchmark Goal

Demonstrate the performance improvement from eliminating unnecessary memory
copies when a queue grows while many elements are appended.

## Optimization Type

- **Category:** Memory Management Inefficiency
- **Rationale:** The original code reallocated the queue on every `ensure` call,
  even when the current capacity was sufficient. This resulted in repeated memory
  allocations and copies. The optimized version checks the capacity and doubles
  it only when needed, reducing the number of allocations dramatically.
