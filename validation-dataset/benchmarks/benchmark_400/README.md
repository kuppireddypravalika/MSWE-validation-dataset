# Benchmark 400: OpenMP parallelization of ReLU

This benchmark is based on the Caffe deep learning framework where the forward pass of the ReLU layer was parallelized using OpenMP.

- **Commit:** [3e9a226](https://github.com/BVLC/caffe/commit/3e9a226aca3794faf27edcfd4240b77d1df3e9f0)
- **Author:** Jongsoo Park
- **Optimization:** Added `#pragma omp parallel for` to the element-wise loop computing the ReLU activation.

## Benchmark Goal
Measure the speedup from parallelizing a large ReLU operation with OpenMP.
