# Benchmark 977: convertFromAscii fast path

This benchmark is based on [Qt](https://github.com/qt/qtbase) commit `0cffd659` which
replaced the hand written ASCII conversion in
`QAbstractConcatenable::convertFromAscii()` with a call to
`QUtf8::convertToUnicode()`. The helper function provides a SIMD optimized
ASCII fast path, avoiding per-character branches in the old implementation.

The benchmark converts large ASCII strings to UTF-16 using both the original
and optimized implementations. The optimized version should complete faster
thanks to the specialized conversion routine.
