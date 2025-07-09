# Benchmark 217: llvm::sort vs std::sort shuffle wrapper

This benchmark is derived from LLVM commit [0e8847398ed3](https://github.com/llvm/llvm-project/commit/0e8847398ed3d9a9918b3844f1cd652979bcd3d1) which replaced a call to `std::sort` with `llvm::sort` in `ISLTools.cpp`.

The LLVM project introduced wrappers around `std::sort` that intentionally shuffle
containers before sorting to help detect nondeterminism.  To benefit from that
infrastructure, Polly switched to using `llvm::sort` instead of directly invoking
`std::sort`.

The code change is very small but highlights the overhead of the shuffle wrapper.
This benchmark mimics the sorting of many small containers and measures the
impact of calling `std::sort` with an extra shuffle step versus calling
`llvm::sort` directly.
