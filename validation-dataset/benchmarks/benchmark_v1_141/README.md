# Benchmark v1 141: OpenMP parallelization of ReLU

This benchmark is based on a change to the Caffe deep learning framework that parallelized the ReLU forward pass using OpenMP.

- **Commit:** [3e9a226](https://github.com/BVLC/caffe/commit/3e9a226aca3794faf27edcfd4240b77d1df3e9f0)
- **Author:** Jongsoo Park
- **Optimization:** Added `#pragma omp parallel for` around the element wise loop in `ReLULayer::Forward_cpu`.

## Benchmark Goal
Measure the speedup from enabling OpenMP parallelism when computing ReLU over large tensors.
