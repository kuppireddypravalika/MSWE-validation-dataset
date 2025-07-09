# Benchmark 267: LayoutPass ordinal override map optimization

This benchmark is based on LLVM commit [1ecf890](https://github.com/llvm/llvm-project/commit/1ecf8902150ae57a20487be67b06128a57fb23cd) which changed `LayoutPass::buildOrdinalOverrideMap` in LLD. The original implementation walked the follow-on chain for every atom regardless of whether the atom already had an ordinal, resulting in quadratic complexity. The patch added a lookup to skip atoms that were already in the map, reducing the runtime to linear.

The benchmark constructs a long follow-on chain of atoms and repeatedly builds the ordinal override map using both versions. The workload is tuned so the unoptimized code runs for several seconds, making the improvement measurable.
