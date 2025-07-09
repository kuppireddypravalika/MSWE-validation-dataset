# Benchmark 163: Lookup in Driver Directory

This benchmark is based on LLVM clang commit [16c7063](https://github.com/llvm/llvm-project/commit/16c706343503911d2ec5e994d9b8fd3f83ac339e) by Gheorghe-Teodor Bercea. The patch enables searching for helper executables in the driver directory so that tools like `clang-offload-bundler` can be discovered when the compiler is invoked from custom scripts.

The optimization simply appends the driver directory to the list of program search paths during `CudaToolChain` construction. This avoids an expensive search through numerous fallback directories.

## Benchmark Goal
Demonstrate that adding the driver directory to the search paths reduces the time spent locating an external tool.
