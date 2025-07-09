# Benchmark 1975: boolean equivalence short-circuit

This benchmark is based on [MiniZinc](https://github.com/MiniZinc/libminizinc) commit `3ec7f533` titled "Short cut compilation for x=true expressions". In MiniZinc's `flatten.cpp` the `BOT_EQUIV` case originally evaluated both operands through the costly `flat_exp` function. The patch adds checks using `istrue` so that if either operand is a constant `true`, the other operand is flattened directly. This avoids unnecessary work when boolean literals appear.

`flatten.hpp` implements a minimal subset of the compiler's expression hierarchy. The `heavy_flat_exp` function simulates the expensive flattening work done in the real code. The functions in `original.cpp` and `optimized.cpp` mirror the control flow of MiniZinc's `flatten_equiv` before and after the patch, enabling us to measure the benefit of the early constant checks.

The harness runs each implementation many times; the default workload of 150000 iterations ensures the original version executes for over ten seconds on the test system.
