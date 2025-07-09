# Benchmark 282: OpenCL kernel build flags

This benchmark is based on commit [`a006f38d`](https://github.com/vampire-team/vampire/commit/a006f38d1086f7730e3d9fc642e6dac2761cf4f9) which added optimisation flags when building OpenCL kernels:

```
-cl-mad-enable
-cl-fast-relaxed-math
-cl-no-signed-zeros
```

These options enable fused multiply-add and relaxed math, providing faster kernel execution. The original implementation omitted these flags.

The benchmark simulates building several kernels and running a computational workload that benefits from the fast math flags.
