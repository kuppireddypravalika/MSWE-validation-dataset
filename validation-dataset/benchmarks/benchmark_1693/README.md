# Benchmark 1693: OpenMP dynamic scheduling for DEM search

This benchmark is derived from the Kratos `DEM_FEM_Search` optimization in [commit 569cdde](https://github.com/KratosMultiphysics/Kratos/commit/569cdde4a6f01b898754a4a804c1d3846dafa176). The patch adds a `schedule(dynamic, 100)` clause to the OpenMP `for` loop that processes particles. The goal is to balance threads when workloads per particle vary.

The microbenchmark models a simplified particle search. Each thread iterates over a list of `SphericParticle` objects and performs an expensive neighbor search using temporary vectors. To create workload imbalance, roughly the first quarter of particles perform a much heavier computation. The original implementation uses the default static schedule while the optimized version specifies dynamic scheduling with a chunk size of 100.

The benchmark measures the time to repeatedly search a set of particles. Both implementations produce the same sum of neighbor counts.
