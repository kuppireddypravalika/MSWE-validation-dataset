# Benchmark 177: Cache language in pango_context_get_metrics

This benchmark is based on the wxWidgets commit [a8c5e1a](https://github.com/wxWidgets/wxWidgets/commit/a8c5e1a9d1f56190e2d87aadc40a75589585af1f). The change avoids passing `NULL` to `pango_context_get_metrics` and instead reuses the context language via `pango_context_get_language`. This prevents the Pango library from recomputing metrics for the default language on each call.

The benchmark replicates the performance impact by simulating expensive work inside `pango_context_get_metrics` when the language parameter is `NULL`. The optimized version supplies the cached language value, reducing the workload dramatically.
