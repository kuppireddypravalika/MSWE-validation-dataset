# Benchmark 264: std::pow dispatch for dynamic base

This benchmark reproduces commit d20d268 from the [Empirical](https://github.com/devosoft/Empirical) project. The patch introduces a check using `__builtin_constant_p` so that `emp::Pow()` calls `std::pow` when the base argument is not known at compile time. The previous implementation always used custom constexpr helper routines even for runtime values.

The benchmark measures the difference when repeatedly exponentiating random values. The optimized version detects dynamic bases and lets the standard library handle them, whereas the original version performs a slower custom calculation.
