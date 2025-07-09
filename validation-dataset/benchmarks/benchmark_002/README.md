
# Benchmark 002: VTK cell ID retrieval optimization

This microbenchmark mimics a performance fix from [OGS](https://github.com/ufz/ogs) where repeated `GetCell()` calls on a `vtkUnstructuredGrid` were replaced by a single `GetCellPoints()` invocation. The change avoided many redundant lookups when converting VTK meshes.

Our simplified benchmark models this behaviour with a custom grid class. The `original` version repeatedly fetches a cell for each point ID while the `optimized` version fetches all point IDs once. The workload is scaled to run for several seconds so the difference is measurable.

