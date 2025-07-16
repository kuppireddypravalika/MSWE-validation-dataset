# Benchmark 7: Preallocate coboundary iterators

This benchmark is derived from [ripser](https://github.com/Ripser/ripser) commit `4fc9585` which optimized the constructor of `simplex_coboundary_enumerator` for sparse distance matrices. The original version appended neighbor iterators using `push_back`, causing repeated reallocations. The optimized version pre-sizes the iterator vectors and assigns each element directly, avoiding these allocations.
