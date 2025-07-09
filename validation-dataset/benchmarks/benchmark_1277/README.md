# Benchmark 1277: Slater integrals size check

This benchmark originates from commit `e93c4632` which changed the two-body
Slater integral routine in the Ambit project. The original implementation always
entered an OpenMP parallel region when merely computing the number of integrals
(`check_size_only` mode). In this mode each thread inserted keys into a shared
set protected by a critical section, causing significant overhead.

The patch runs the parallel region only when full integrals are required and
removes the critical section while checking sizes. This serializes the cheap
workload and avoids synchronization costs.

The benchmark reproduces the nested loops that generate integral keys. In
performance mode it executes with `check_size_only=true`. The original version
uses a parallel region and critical section while the optimized version executes
serially, demonstrating the performance benefit of the change.
