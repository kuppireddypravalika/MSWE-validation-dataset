# Benchmark 348: Lock removal using memory fence

This benchmark is derived from LLVM commit [0b7401d](https://github.com/llvm/llvm-project/commit/0b7401d3cdb4a2ecd266396ef1a77c8533a4476d) which removed a mutex in `SelectionDAG::getValueTypeList`. The original code locked a global mutex every time a value type was requested. The patch observed that writes to the shared array are always identical and therefore replaced the lock with a memory fence, greatly reducing contention when accessed from multiple threads.

The microbenchmark models this behaviour with a global table updated concurrently by many threads. The `original` implementation protects the table with a `std::mutex`, while the `optimized` version performs an unlocked write followed by `std::atomic_thread_fence`.

## Benchmark Goal

Measure the performance gain from removing the lock when many threads repeatedly update the same entries.
