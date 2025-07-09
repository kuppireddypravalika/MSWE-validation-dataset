# Benchmark 394: StructurizeCFG skip uniform regions

This benchmark is derived from [GPUOpen-Drivers/llpc commit f9eee6bff6](https://github.com/GPUOpen-Drivers/llpc/commit/f9eee6bff6717c0754afacc67659106f2bd010e6).
The patch restores shader performance by enabling the LLVM StructurizeCFG option
`SkipUniformRegions` for graphics pipelines. Without this option the compiler
performs expensive analysis on regions of uniform control flow which can slow
down compilation.

Our microbenchmark simulates this behaviour by repeatedly processing a large
array. When `SkipUniformRegions` is disabled each element of every uniform block
is handled individually. When the option is enabled the uniform block is
collapsed to a single operation. The result is identical but the optimized
implementation avoids many redundant computations.
