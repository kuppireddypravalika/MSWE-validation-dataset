# Benchmark 178: Skip gen_expr for automatic variables

This benchmark is based on commit cffba0a7cafeed34d6b2a4c5f29e5df1026c6687 which modified the `gen_void` helper in Open Watcom's optimizer. The change avoids calling `gen_expr` when the expression node represents an automatic variable (`en_auto`). Variables declared inside a `for` statement can be optimized away and emitting code for them was unnecessary overhead.

The benchmark reproduces the performance impact of skipping this extra call.
