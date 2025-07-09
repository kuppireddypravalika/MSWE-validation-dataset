# Benchmark 350: ArgCast handles declared functions

This benchmark is derived from LLVM's poolalloc project commit [9d4dab1](https://github.com/llvm-mirror/poolalloc/commit/9d4dab1b90a4dbcf709fad6f5186db9f69bccc4c) where the `ArgCast` pass stopped ignoring functions that were only
forward declarations. By removing the `isDeclaration` check, the pass can
also optimize calls to such declared functions, reducing unnecessary argument
casts.

The microbenchmark simulates the pass's behaviour on a module containing many
declared functions. The original version skips these functions during
processing, leaving their call sites untouched and slower. The optimized
version processes them as well, resulting in faster call execution.
