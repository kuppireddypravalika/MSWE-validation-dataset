# Benchmark 308: Variable ordering for enumeration

This benchmark is derived from commit [0bfb59a](https://github.com/mCRL2org/mCRL2/commit/0bfb59a0d5a59ba6ddf1c19e39fc7ea369123685) of the mCRL2 project.
The patch applied a heuristic ordering of summation variables before enumerating
state spaces. By enumerating variables with expensive domains first, a large
performance improvement was achieved (36s down to 24s on a real model).

Our simplified benchmark replicates this scenario with a small enumeration
routine. The original implementation processes variables in their initial order
while the optimized version sorts them by cost before enumeration.

## Benchmark Goal

Measure the benefit of ordering variables for enumeration. The program executes a
nested loop structure with one expensive variable. When this variable appears
last (original version) the inner heavy computation is executed many more times
than when it is placed first (optimized version).

