# Benchmark 1025: Remove OpenMP from tile extraction loops

Derived from [commit a90cf87](https://github.com/eth-cscs/spla/commit/a90cf8727fbf84d29151d297ba90bfd5a3f8b0a0)
in the SPLA project. The patch removed `#pragma omp parallel` statements from the
`TileHost::extract` routine because spawning threads for very small matrix blocks
resulted in slower execution.

This benchmark models a simplified `TileHost` that copies sub blocks from a
temporary tile into the result matrix. The original version parallelises the
copy with OpenMP while the optimized version performs the loops serially.
The benchmark repeatedly calls the extraction routine to highlight the overhead
of launching threads on small workloads.
