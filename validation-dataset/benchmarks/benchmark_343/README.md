# Benchmark 343: Prevent redundant attribute copy

This benchmark is based on PaddlePaddle commit [4ea2330](https://github.com/PaddlePaddle/Paddle/commit/4ea23307598206c7d1bd0f6a8dcb10997b399d1b). The patch avoids copying operator attributes when MKL-DNN is disabled by checking the global `FLAGS_use_mkldnn` flag. In the original implementation the attribute map was copied on every call, even if MKL-DNN was not used, adding unnecessary overhead.

The benchmark replicates this behavior with a simplified operator that stores a map of attributes. Each invocation optionally copies a large attribute map depending on the flag. The workload highlights the difference in runtime when the copy is gated by the flag.
