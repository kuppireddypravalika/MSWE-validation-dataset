# Benchmark 261: QMutexLocker constructor inlining

This benchmark is derived from Qt's commit [38dc1f7](https://github.com/qt/qtbase/commit/38dc1f7597c66e205e5ece0e677187ea2dfa8052) which optimised `QMutexLocker` by replacing a call to `relock()` in the constructor with direct locking logic. The change avoids unnecessary function call and conditional checks when a mutex is provided.

The benchmark measures the overhead of constructing `QMutexLocker` objects repeatedly. The original implementation calls `relock()` which checks and locks the mutex. The optimized version performs the lock directly in the constructor.
