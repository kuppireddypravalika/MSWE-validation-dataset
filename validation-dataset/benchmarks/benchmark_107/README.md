# Benchmark 107: sort unchecked wrapper

This benchmark is derived from [flux](https://github.com/tristanbrindle/flux) commit `eaadbcee` which changed the implementation of `flux::sort` to wrap the input sequence with `unchecked(ref(seq))` before calling `pdqsort`. The original version performed bounds checking on each element access during sorting, while the optimized version disables these checks via the wrapper.

The benchmark models a `Sequence` container with checked iterators. The original `sort` uses these iterators directly, incurring bounds checks. The optimized version wraps the sequence to obtain unchecked iterators, reducing overhead.
