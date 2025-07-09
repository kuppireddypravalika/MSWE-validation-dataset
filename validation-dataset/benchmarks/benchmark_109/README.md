# Benchmark 109: Optimized single-byte memset

This benchmark reproduces a performance fix from the [Tracy profiler](https://github.com/wolfpld/tracy) project. The original code always used `memset` when marking memory pages. Commit [6d40502](https://github.com/wolfpld/tracy/commit/6d40502068aa9648dddf51b7dc2fe5290a71441e) adds a special case when only a single byte needs to be written, replacing `memset` with a direct assignment.

The microbenchmark models the `GetMemoryPages()` logic before and after the change and measures the effect of the optimization.
