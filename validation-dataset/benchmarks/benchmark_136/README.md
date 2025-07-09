# Benchmark 136: Enable OpenCL FP_CONTRACT

This benchmark is derived from [Halide](https://github.com/halide/Halide) commit `fe2822e`, where the OpenCL code generator was modified to emit
`#pragma OPENCL FP_CONTRACT ON`. The change allows fused multiply-add operations
in generated kernels, improving performance of floating point arithmetic.

Our simplified benchmark models this by generating a dummy OpenCL source string
with or without the pragma and running a large number of multiply-add
operations. The optimized version uses `std::fma` to mimic the fused operation.
