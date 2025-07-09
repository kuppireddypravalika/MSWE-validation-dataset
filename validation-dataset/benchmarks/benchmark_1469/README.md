# Benchmark 1469: VectorGetLow shift order

This benchmark originates from Crypto++ commit [8c21b6a](https://github.com/weidai11/cryptopp/commit/8c21b6af056d297125adbed7190a02afc32c6f40) which modified the implementation of `VectorGetLow`. The original version shifted the value right then left, while the optimized version reverses the order. This produces the correct low 64 bits and avoids unnecessary work on big-endian machines.

The benchmark models a simplified PowerPC SIMD helper. It repeatedly extracts the low half of a 16‑byte vector using both the original and optimized sequences of byte shifts.
