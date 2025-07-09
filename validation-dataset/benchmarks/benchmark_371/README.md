# Benchmark 371: LLVM ListInit FoldingSet optimization

This benchmark reproduces a performance fix from LLVM commit [432dc9c](https://github.com/llvm/llvm-project/commit/432dc9cc78c0be0092b123d36fca7893484d1b79). The patch avoids costly `FoldingSet` lookups in `ListInit::convertInitializerTo` when no element changes.

Our minimal recreation implements a small type system and a global list pool to mirror LLVM's behavior. The original implementation always searches the pool even when converting a list to an identical type. The optimized version returns the existing list directly when nothing needs changing.
