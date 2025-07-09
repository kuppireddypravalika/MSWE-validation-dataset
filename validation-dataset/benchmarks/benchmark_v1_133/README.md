# Benchmark v1_133: Thread-local cache for TThread::Self

This benchmark is based on commit [ffd52ac](https://github.com/root-project/root/commit/ffd52ac515e7e78fb581d364e3ba9d1f5e2f7c05) from the **ROOT** project. The patch introduced a thread-local pointer inside `TThread::Self` to avoid linearly searching the global thread list on every call.

## Benchmark Goal
Show the performance benefit of caching the current `TThread` pointer in thread-local storage rather than repeatedly traversing the list of all threads.
