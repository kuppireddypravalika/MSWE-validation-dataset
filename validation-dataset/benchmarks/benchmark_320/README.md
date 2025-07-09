# Benchmark 320: Thread-local LOS cache

This benchmark is based on commit [9dab5e3](https://github.com/polserver/polserver/commit/9dab5e3601e66f0161954aef1ac68f9765f1864d) from the **POL server** project. The patch introduced a conditional `thread_local` cache for line of sight checks. Older MSVC compilers lacked full support for thread-local storage, so the code fell back to a normal local variable.

Reusing a thread-local cache avoids repeated vector allocations when computing line of sight between objects.

## Benchmark Goal

Show the performance difference between constructing a fresh cache on every call and using a persistent `thread_local` cache.
