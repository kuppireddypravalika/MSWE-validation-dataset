# Dataset 166 Benchmark: Preallocate vector in Top-K loop

This benchmark originates from [PaddlePaddle](https://github.com/PaddlePaddle/Paddle) commit `4d55aca` which added a `reserve` call when building a temporary vector inside the top-k kernel. The original implementation resized the vector implicitly during `push_back` for each element. The optimized version reserves enough capacity for the entire row, avoiding repeated allocations and improving performance.

The benchmark mimics the kernel by sorting each row of a matrix and selecting the top `k` values. The original version allocates the vector without reserving capacity, while the optimized version preallocates it.
