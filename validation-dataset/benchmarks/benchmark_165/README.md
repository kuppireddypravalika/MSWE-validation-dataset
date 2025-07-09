# Benchmark 165: movprfx optimization for VBSL

This benchmark is based on the optimization from commit [d7c6ed8](https://github.com/FEX-Emu/FEX/commit/d7c6ed842dfcfc34b9454ebb219f3ce345676c7e) in the FEX-Emu project. The patch replaces a `mov` instruction with `movprfx` before a `bsl` operation so that CPUs supporting this fusion can execute the copy and bitwise select in a single instruction pair.

The microbenchmark models the same behavior on large integer arrays. The `original` implementation first copies the "true" vector and then performs a separate bitwise select loop. The `optimized` version performs the selection directly without the extra copy. Both produce identical results but the optimized version avoids the redundant memory move.
