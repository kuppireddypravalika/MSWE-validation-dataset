# Benchmark 351: BitVector XOR optimization

This benchmark is derived from the MOTION secure computation framework commit [cf5e6b2](https://github.com/encryptogroup/MOTION/commit/cf5e6b22051f3992ffdc3547230aa6693ba8ee08). The change simplified `BitVector::operator^=` by removing unnecessary resizing and using `std::transform` for the XOR loop.

- **Commit:** [cf5e6b2](https://github.com/encryptogroup/MOTION/commit/cf5e6b22051f3992ffdc3547230aa6693ba8ee08)
- **Author:** Lennart Braun
- **Optimization:** Use `std::transform` to XOR byte arrays without resizing.

The benchmark creates large `BitVector` instances and repeatedly applies the XOR operator to measure the performance difference.
