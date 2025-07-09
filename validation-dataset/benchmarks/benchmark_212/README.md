# Benchmark 212: lld parallel_for_each chunking removal

This benchmark is based on LLVM commit [7af634c](https://github.com/llvm/llvm-project/commit/7af634cad97df00ecfc3175e518bd19fe0b91e78) which modified `parallel_for_each` in `lld/Core/Parallel.h`.

The original implementation spawned tasks in large chunks of 1024 elements. When the range was smaller than this chunk size, all work executed sequentially. The patch changed the function to spawn one task per iteration, enabling parallelism even for small ranges.

The benchmark simulates this behaviour using a simple `TaskGroup` that launches threads. Each element performs heavy trigonometric work so the sequential version runs for over ten seconds. The optimized version spawns a task for every element, dramatically reducing run time.
