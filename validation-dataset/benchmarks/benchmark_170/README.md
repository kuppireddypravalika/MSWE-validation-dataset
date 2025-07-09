# Benchmark 170: Avoid redundant error string allocation

This benchmark is based on [LLVM lld commit 9589f84](https://github.com/llvm/llvm-project/commit/9589f84180671bbf395ce28396d9e8fe09d562e0).
The patch removes an unnecessary construction of an error message when symbol name lookup succeeds in `ObjFile::createSymbol`.

The original code passed the expression `check(Sym->getName(StringTable), toString(this))`,
which always constructed an error message string regardless of whether an error occurred.
The optimized code retrieves the name first and only formats the error message when needed, reducing memory allocations and runtime.

This microbenchmark simulates repeated symbol name lookups and measures the
cost difference between allocating an unused error string (original) and only
creating it on failure (optimized).
