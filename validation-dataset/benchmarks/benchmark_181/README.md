# Benchmark 181: Skip zero padding in LLVM X86 Printer

This benchmark is based on [LLVM commit 59068a0](https://github.com/llvm/llvm-project/commit/59068a0d713d9a8d540d06dde8316d7cf47e1da3) by Chris Lattner.
The patch avoids emitting the assembly directive `.zero 0` when printing
constant structs. Omitting these redundant lines reduces assembly output
size and slightly improves compilation speed.

The benchmark reproduces the printing loop that previously wrote
`.zero 0` for every struct field even when no padding was required.
The optimized version checks `padSize` before emitting the directive.
