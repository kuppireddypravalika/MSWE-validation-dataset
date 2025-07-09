# Benchmark 392: Adam to SGD Optimizer Change

This benchmark is derived from commit [118e5de](https://github.com/pytorch/examples/commit/118e5de18e6afef3c9cb3a2a4a0d7c8724e7e316) of a semantic segmentation example. The patch replaced an Adam optimizer with SGD and increased the training batch size from 6 to 8.

The benchmark simulates a simple training loop for logistic regression. The original version uses Adam with a batch size of 6. The optimized version switches to SGD with momentum and a larger batch size of 8. Both versions perform the same amount of work and produce identical numeric output (sum of trained weights).

