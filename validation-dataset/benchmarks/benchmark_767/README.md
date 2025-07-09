# Benchmark 767: OpenMP parallelization of batched GEMM

This benchmark originates from [ctranslate2](https://github.com/OpenNMT/CTranslate2) commit `17ccba4a` which parallelized the function `primitives<Device::CPU>::gemm_batch` when MKL is not used. The patch adds `#pragma omp parallel for` before the loop iterating over each matrix pair.

The simplified benchmark implements a batched matrix multiplication routine where each batch element performs a standard GEMM. The `optimized` version parallelizes the outer loop using OpenMP, while the `original` version runs sequentially. Workload parameters are chosen so the sequential implementation executes for over ten seconds, highlighting the performance gain from parallelization.

