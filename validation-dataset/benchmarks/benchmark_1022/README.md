# Benchmark 1022: bitscanforward intrinsic

This benchmark is derived from [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2) commit `6cf6c4b` which optimized the `bitscanforward` utility function. The original implementation scanned each bit sequentially to find the first set bit. The optimized version uses the compiler intrinsic `_BitScanForward` on Visual Studio, greatly reducing the number of instructions.

The benchmark replicates this behaviour with a simplified implementation. The optimized version uses `__builtin_ctz` when available to mirror the intrinsic usage.
