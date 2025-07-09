# Benchmark 244: Precompute Use-Def Info in Copy Propagation

This benchmark is derived from an optimization in the Eclipse OMR compiler ([commit 00dd94c](https://github.com/eclipse-openj9/openj9/commit/00dd94c70812aec35d390fc2cdeb8380cf3e1cfb)) which improved compile-time performance of the copy propagation pass. The original implementation repeatedly computed use-def information on demand, while the patch precomputes and caches this data before running the propagation loop.

Our simplified benchmark models a similar scenario where a `UseDefInfo` structure lazily gathers use sites for each definition. The `optimized` version explicitly builds this table once, avoiding repeated scanning when querying uses.
