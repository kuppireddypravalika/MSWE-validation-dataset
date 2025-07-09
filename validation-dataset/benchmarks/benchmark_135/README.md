# Benchmark 135: Custom isnan implementation

This benchmark is based on [commit 7680eca](https://github.com/SamHocevar/lolengine/commit/7680eca5d8caa4ea24eacffbf0430f604f86e5f4) from the Lol Engine project.
The patch introduced a handcrafted `isnan()` check using bit operations
in place of a naive comparison approach. Besides working with
`-ffast-math`, it also provided a noticeable speed‑up for large arrays of
floating‑point values.

The benchmark replicates this scenario by repeatedly counting NaN values
in a large vector. The original version performs the comparison through
`x == x` on a volatile copy, whereas the optimized version inspects the
raw floating‑point bits.
