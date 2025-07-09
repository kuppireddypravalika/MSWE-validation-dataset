# Benchmark 259: DominatorTree properlyDominates optimization

This benchmark is based on LLVM commit [a29dbd2](https://github.com/llvm/llvm-project/commit/a29dbd2dcc04c8d07dd9e1e49b4e54debbc23996) which replaced a call to `DominatorTree::dominates` with `properlyDominates` in `RegionInfo::isRegion`. The change avoids unnecessary dominance checks when the successor is the entry block itself.

The microbenchmark models a region check that iterates over many successor blocks. In the original version the expensive `dominates()` query is performed even when the successor equals the entry block and the result is discarded afterwards. The optimized version uses `properlyDominates()` which returns `false` immediately for that case, saving work.
