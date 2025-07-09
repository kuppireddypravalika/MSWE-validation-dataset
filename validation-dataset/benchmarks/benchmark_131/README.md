# Benchmark 131: Bit vector allocation delay in LocalCSE

This benchmark replicates a performance patch from OpenJ9's optimizer component.
The original implementation pre-allocated a number of `TR_BitVector` instances
based on the total symbol and node counts of a compilation. The patch delays
this allocation by initializing these bit vectors with size `0` and allowing
them to grow lazily. This avoids large initial allocations for mostly sparse
data structures and improves compiler pass startup time.

- **Commit:** c030ce9ea9732b30eae37ac061772affc9e72602
- **Link:** https://github.com/eclipse/openj9/commit/c030ce9ea9732b30eae37ac061772affc9e72602
- **Optimization:** avoid upfront allocation of large bit vectors in `prePerformOnBlocks`.
