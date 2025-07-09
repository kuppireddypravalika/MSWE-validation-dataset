# Benchmark 032: tick_send lock removal

This benchmark derives from RetroShare commit `28458bf1` which moved a mutex lock in `pqistreamer::tick_send`.
The original implementation locked `mStreamerMtx` before calling the potentially blocking `mBio->cansend()`.
The optimized version performs the `cansend` check without holding the lock and only locks when sending data.
This reduces contention when multiple threads invoke `tick_send` concurrently.
