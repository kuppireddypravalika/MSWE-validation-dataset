# Benchmark 137: String literal merging with ASan

This benchmark models an optimization from LLVM Clang commit 5083d1e70982
which allowed string literals to be merged even when AddressSanitizer is
enabled. The patch removed an extra condition preventing mangling in
`GetAddrOfConstantStringFromLiteral` so identical literals share a
comdat section, reducing binary size.

Our benchmark simulates this by conditionally caching large string
allocations. The original implementation bypasses the cache when ASan is
"enabled", while the optimized version reuses the cached allocation.
