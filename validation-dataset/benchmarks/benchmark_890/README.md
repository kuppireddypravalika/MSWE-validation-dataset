# Benchmark 890: atomic increment inside critical

Derived from [commit a78450c](https://github.com/rk221/ReadKmerHash/commit/a78450cb15d411d76fcd6fa0e8079433e8a70b4a) which modified `hash_sequences` to add `#pragma omp atomic` inside a `#pragma omp critical` region when updating hash depth maps. The change fixed macOS segfaults but slowed execution due to unnecessary atomic operations.

The benchmark emulates parallel hashing of sequences and map updates. The original version includes both `critical` and `atomic` directives while the optimized version uses only the `critical` section.
