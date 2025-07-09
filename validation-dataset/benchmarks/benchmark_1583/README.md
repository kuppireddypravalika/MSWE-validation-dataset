# Benchmark 1583: depth results parallel loop

This benchmark is based on [commit f677d6d](https://github.com/OPM/opm-simulators/commit/f677d6d60082dcd1430b57dc4b85aaf3895869fa)
which parallelized the loop computing depth related results using OpenMP.
The original implementation iterated sequentially over all reservoir cells,
while the patched version adds `#pragma omp parallel for` to distribute the
work across threads.

The benchmark models a simplified grid of cells and replicates the per-cell
computations performed in `computeDepthRelatedResults`. The optimized version
applies the OpenMP pragma to the main loop so the results are computed in
parallel.

