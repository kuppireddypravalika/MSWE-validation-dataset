# Benchmark 353: Skip manual pthread stack with ASAN

This benchmark is derived from [commit e63e50f](https://github.com/scylladb/seastar/commit/e63e50f2e6a2ede957dcc350a43c19a19b62eab6) in the Seastar project.
The patch avoids calling `pthread_attr_setstack` and the associated `mmap` allocations when
AddressSanitizer is enabled. Using custom stacks interferes with ASAN's crash
reports and is unnecessary for debugging builds.

The benchmark creates many lightweight threads to highlight the overhead of
manually allocating stacks compared to using the default pthread stack when
`SEASTAR_ASAN_ENABLED` is defined.
