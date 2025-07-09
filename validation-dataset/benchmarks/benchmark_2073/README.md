# Benchmark 2073: ScalarEvolution max trip count shift

Derived from LLVM commit `3964acc2` which updated the backedge trip count
calculation in `ScalarEvolution::HowManyLessThans` to use the helper functions
`GetMinSignBits` and `GetMinLeadingZeros`. These functions tighten the computed
maximum end value by shifting the theoretical type maximum based on the known
sign or leading zero bits of the limit value.

The original implementation always used the full signed/unsigned maximum,
producing overly large maximum trip counts. The optimized version uses the new
bit information to reduce this bound.

This benchmark models the loop count computation and consumes the resulting
maximum backedge count to demonstrate the difference in runtime between the two
approaches.
