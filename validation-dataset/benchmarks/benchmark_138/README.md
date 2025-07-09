# Benchmark 138: Explicit log2<double> to avoid integer slowdown

This benchmark reproduces a performance issue fixed in
[f57cec2](https://github.com/opencog/opencog/commit/f57cec2a317be64128d5949df6830ff47b028750).
The original implementation of `information_theoretic_bits` called a
templated `log2` function with integer arguments. Because the integer
specialization performed iterative bit counting, this slowed down the
calculation. The patch explicitly invokes `log2<double>` so that the
fast floating point implementation is used.

The benchmark constructs field sets with many entries and repeatedly
computes the information theoretic bits using both the original and
optimized versions.
