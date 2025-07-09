# Benchmark 263: Fix OpenMP pragma typo

This benchmark originates from the RELION project, where a misspelled OpenMP directive prevented a critical loop from running in parallel.

- **Commit:** [45e09aa](https://github.com/3dem/relion/commit/45e09aae3ab43687e2dbfc30d97610ff0047dadd)
- **Author:** Takanori Nakane
- **Optimization:** Corrected `#pragma omp prallel` to `#pragma omp parallel`, enabling multithreaded execution.

## Benchmark Goal
Demonstrate the performance impact of losing OpenMP parallelism due to a simple typo.
