# Benchmark 1402: Top K OpenMP parallelization

This benchmark is based on [PaddlePaddle](https://github.com/PaddlePaddle/Paddle) commit `e7f06965` which added an OpenMP pragma to parallelize the outer loop in `top_k_op.h`. The original implementation processed each row sequentially, while the optimized version uses `#pragma omp parallel for` (guarded by `PADDLE_WITH_MKLDNN`) to run rows in parallel.

The benchmark mimics a simplified top-k operation over a 2D tensor. The optimized version leverages OpenMP when `PADDLE_WITH_MKLDNN` is defined, reducing total runtime.
