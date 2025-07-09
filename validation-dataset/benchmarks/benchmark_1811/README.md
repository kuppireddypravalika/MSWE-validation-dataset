# Benchmark 1811: SSE4 sign clamp

This benchmark is derived from [PCSX2](https://github.com/PCSX2/pcsx2) commit [0d519a5](https://github.com/PCSX2/pcsx2/commit/0d519a5f28d963246121dfa9249f50a1a2580d20) which optimized the `fpuFloat3` helper used in the x86 recompiler. The patch replaced a sequence of SSE instructions with two SSE4 `pminsd`/`pminud` operations to clamp floats to the representable range while preserving the sign of NaNs.

The original implementation copied the value to a temporary register, masked the sign bit, performed scalar min/max clamps and finally restored the sign. The optimized version performs the clamping directly using integer min operations.
