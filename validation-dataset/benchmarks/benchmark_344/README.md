# Benchmark 344: Boost.Asio single-threaded optimization

This benchmark reproduces commit [6da6f1b](https://github.com/TrinityCore/TrinityCore/commit/6da6f1b415be2e7964c7c15c87b29a38052e76e4) from the *TrinityCore* project.
The patch initializes `boost::asio::io_service` with a concurrency hint of `1`
to enable single-threaded optimizations when the network thread runs without
additional workers.

The original implementation constructed `io_service` with the default
constructor, which keeps internal locking enabled. By explicitly passing `1`,
Boost.Asio skips unnecessary mutex operations, improving throughput of posted
tasks.

Our minimal reproduction repeatedly posts a self-rescheduling handler into the
`io_context`. The workload is sized so the original version runs for over ten
seconds.
