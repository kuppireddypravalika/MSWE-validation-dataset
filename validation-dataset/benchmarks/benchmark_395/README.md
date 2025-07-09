# Benchmark 395: ScalarEvolution max trip count precision

Derived from [LLVM commit 3964acc](https://github.com/llvm/llvm-project/commit/3964acc2f44ee02cdd4e2a54a5232537bdf3ee73) which updated
`ScalarEvolution` to use `GetMinSignBits` and `GetMinLeadingZeros` when
estimating the maximum trip count of a loop. The change tightens the
upper bound which avoids needless iteration during analysis.

This benchmark models the original compile-time behaviour with a simplified
loop analysis routine. The unoptimized version assumes the widest possible
end value, causing a large inner loop to run. The optimized version shifts
the bound based on leading zeros of the end value, dramatically reducing
the work while producing the same numeric result.
