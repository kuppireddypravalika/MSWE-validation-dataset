# Benchmark 218: Avoid over-copying in APFloat::makeNaN

This benchmark is based on LLVM commit [d44c6cc](https://github.com/llvm/llvm-project/commit/d44c6cc4776d38dcb91069a776904b307f634370) which fixed a bug in `APFloat::makeNaN`. The original implementation always copied a number of 64‑bit words equal to the target floating point size, even when the provided `APInt` payload was smaller. This could read past the end of the payload and wasted time copying unnecessary memory.

The patch changes the copy length to `std::min(fill->getNumWords(), numParts)`, avoiding the extra work and potential invalid reads.

## Benchmark Goal
Evaluate the performance impact of limiting the copy length when constructing a NaN from an `APInt` payload.

## Optimization Type
- **Category:** Memory Inefficiency
- **Rationale:** The old code unnecessarily copied large amounts of memory from the payload. The fix copies only the provided words, reducing memory traffic.
