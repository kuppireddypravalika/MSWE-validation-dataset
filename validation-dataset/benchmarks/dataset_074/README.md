# Benchmark 074: Topological distance matrix traversal

This benchmark is based on commit `ecf3e8d` from the [Aleph
library](https://github.com/DAGroup/aleph), which rewrote the traversal
of the pairwise distance matrix in `topological_distance.cc`.  The
original implementation iterated over all `n^2` index pairs and skipped
half of them when computing the upper triangular part of the matrix.
The optimized version directly iterates over the `n*(n-1)/2` relevant
pairs and computes the row/column indices from a single loop index.

This change reduces the number of loop iterations and eliminates a
conditional inside the nested loops.
