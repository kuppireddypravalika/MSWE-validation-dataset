# Benchmark 1308: Vector_FToF temporary elimination

This benchmark is derived from [FEX-Emu](https://github.com/FEX-Emu/FEX) commit `5536f1e8` which modified the Arm64 JIT's `Vector_FToF` operation. The original implementation first zipped the input vector into a temporary register before converting element sizes. The patch reuses the destination register for this step, avoiding the extra temporary copy.

The benchmark models the conversion of a large vector of 16‑bit values to 32‑bit floats. The original version copies the data to a temporary buffer before converting, while the optimized version performs the conversion in-place using the destination buffer.
