# Benchmark 337: Qt foreach replacement

This benchmark is derived from commit [9c045b8](https://github.com/GerbilSoft/rom-properties/commit/9c045b86bf6162625a1b0553e1371f420f3ee0fe) in the *rom-properties* project. The patch replaced Qt's `foreach` macro with a C++11 range-based `for` loop in `findDirectChild()` to avoid copying the child list when iterating.

Our microbenchmark models this by iterating over a vector of child objects using either a copied container (original) or a direct reference (optimized). The difference is subtle but results in slightly less work inside a tight loop.
