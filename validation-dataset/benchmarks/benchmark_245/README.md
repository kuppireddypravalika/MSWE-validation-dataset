# Benchmark 245: Triangular sparse matrix multiplication

This benchmark is based on the Blaze commit [`4cc9826`](https://github.com/blaze-lib/blaze/commit/4cc98268e472c76bfb4a114dceaea836ec6b46f8) which optimized the multiplication of column-major sparse matrices with row-major sparse matrices (`TSMatSMatMultExpr`) when the operands are lower/upper triangular.

The original implementation always iterated over the full shared dimension when computing each element. The patch restricts the loop bounds to the relevant triangular region using precomputed `kbegin` and `kend` indices.

The benchmark models this behaviour using simple dense matrices but applies the same loop structure to highlight the reduced work in the optimized version.
