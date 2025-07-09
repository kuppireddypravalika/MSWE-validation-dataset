# Benchmark 237: In-place LLT decomposition

This benchmark is based on [Stan Math commit 5cf225f](https://github.com/stan-dev/math/commit/5cf225f0e3a50195136e4ac6348e326d78ee27be) which switched
an LLT factorization to Eigen 3.3's in-place form. The change avoids copying the
resulting Cholesky factor and improves performance for large matrices.

The benchmark compares an implementation that performs the LLT and then copies
the lower factor with one that performs the decomposition directly in the input
matrix.
