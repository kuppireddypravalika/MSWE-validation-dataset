# Benchmark 2074: dropAllTypeUses caching

Derived from LLVM commit [`4338f905`](https://github.com/llvm/llvm-project/commit/4338f905b79bf996bfa10ba515f915e2986bcccb) which micro-optimized `DerivedType::dropAllTypeUses`.
The original code fetched the `Int32` type from the context on every loop iteration.
The patch cached this pointer outside the loop to avoid repeated lookups.

This benchmark models the function by repeatedly resetting an array of type pointers.
The original version calls `getInt32Ty` for each element, while the optimized version
computes the pointer once and reuses it.

