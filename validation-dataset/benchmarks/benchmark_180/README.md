# Benchmark 180: TVM GenericFunc schedule dispatch

This benchmark derives from [commit a34c88d](https://github.com/pytorch/pytorch/commit/a34c88da857974d8d40ae0858b5bbb8b6513bd9d) in PyTorch. The patch fixed a performance issue when scheduling a quantized matrix multiply by calling the registered `GenericFunc` instead of the generic schedule directly. Without the fix the default schedule was used which is much slower.

The benchmark simulates this scenario with a costly matrix multiplication. The `original` version invokes the generic schedule, while the `optimized` version retrieves the specialized implementation via `GenericFunc::Get`. The workload is scaled so that the original implementation runs for over ten seconds per invocation.

