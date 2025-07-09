# Benchmark 100: Avoiding memset for single byte writes

This benchmark is based on commit [6d40502](https://github.com/wolfpld/tracy/commit/6d40502068aa9648dddf51b7dc2fe5290a71441e) from the *Tracy* profiler project.
The patch optimized the handling of memory page visualization by replacing `memset`
calls with a single direct assignment when only one byte needs to be written.

The original implementation always called `memset` even when the length was
exactly one byte. The optimized version checks this case and writes directly to
avoid the overhead of a function call.

This benchmark replicates that behaviour with a simplified model that repeatedly
updates small portions of a page sized buffer.
