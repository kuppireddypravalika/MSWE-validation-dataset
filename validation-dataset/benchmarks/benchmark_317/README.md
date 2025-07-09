# Benchmark 317: Avoid redundant expression simplification

This benchmark is based on commit [e199394](https://github.com/vtil-project/VTIL-Compiler/commit/e199394e27b4ac6b529f58e1d2086191a90853e7) from the VTIL project. The patch moved a call to `simplify()` before comparing symbolic expressions with `equals()`. Since `equals()` internally triggers simplification when needed, the old order performed two simplification passes on every register state.

Our microbenchmark recreates this scenario with a lightweight `Expression` type whose comparison lazily simplifies unsimplified values. The original version calls `equals()` first and then `simplify()`, causing unnecessary work. The optimized version simplifies once up front before any comparisons.
