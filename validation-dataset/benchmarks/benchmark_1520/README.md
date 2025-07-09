# Benchmark 1520: Threaded BA table evaluation

This benchmark is derived from [QMCPACK](https://github.com/QMCPACK/qmcpack) commit `e1db0234` which parallelized the full batched SoA distance table evaluation. The original implementation computed all pairwise distances sequentially. The patch introduced an OpenMP parallel region and divided the work using `FairDivideAligned` so that threads handle disjoint chunks of the source particles.

The benchmark models minimal particle sets and distance table structures. The `original` version loops over all sources for each target, while the `optimized` version parallelizes the inner work using OpenMP. The harness sets the number of OpenMP threads (default 4) and repeats the table evaluation enough times so the sequential run exceeds ten seconds, clearly demonstrating the performance gain.
