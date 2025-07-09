# Benchmark 209: Removing redundant `true` conjuncts

This benchmark is based on the mCRL2 commit [`eb5273b`](https://github.com/mCRL2org/mCRL2/commit/eb5273bbf3e317f5a2289907f7e88379f8e65afe) which replaced calls to `and_` by `optimized::and_` in the untime algorithm. The optimized version strips `true` operands when building boolean expressions, avoiding deep expression trees.

Our microbenchmark constructs long chains of conjunctions, inserting `true` for half of the operands. The original implementation keeps all nodes while the optimized version immediately eliminates redundant `true` terms. We measure the time to build and evaluate these expressions repeatedly.
