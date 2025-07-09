# Benchmark 302: Avoid temporary APInt creation

This benchmark is based on LLVM commit [64df5b0](https://github.com/llvm/llvm-project/commit/64df5b09176e0d8cd255357c0f8be6a6f4f2c44e) which replaced `APInt(TyBits, 1)` with the cheaper `1` when OR-ing with an `APInt`. Avoiding construction of a temporary `APInt` eliminates dynamic allocation for wide integers.

The simplified code below models repeated checks of
`(KnownZero | 1).isAllOnesValue()` versus `(KnownZero | APInt(TyBits, 1)).isAllOnesValue()`.
The original version constructs a new `APInt` on every iteration while the
optimized version uses the overload that accepts an integer.

