# Benchmark 384: Skip leading zeroes in substring search

Derived from libhtp commit [50cef6d](https://github.com/OISF/libhtp/commit/50cef6d393ffcacb69f42972592d9a54c02d7e9a) which optimizes `bstr_util_mem_index_of_mem_nocasenorzero`.
The original implementation examined each position even when the input contained long runs of `\0` bytes, causing quadratic behavior. The patch skips over leading zeroes before the inner comparison loop, reducing needless iterations.

This benchmark compares the original and optimized versions when searching a large buffer with many leading zero bytes.

The workload is tuned so the unoptimized function runs for over 10 seconds on the CI server.
