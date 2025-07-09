# Benchmark 304: GC interval constant

This benchmark is based on [commit d0bfd9d](https://github.com/Unknown/repo/commit/d0bfd9dc6a262334489b89ded792155d62f6622d) which replaced a hard coded
1-second sleep in a garbage collector thread with a configurable `GCINTERVAL`
value. When the collector wakes more frequently, accumulated work is reduced and
overall throughput improves.

The benchmark simulates a distributed reference counting garbage collector. The
original version waits one second between cleanup passes, while the optimized
version uses a 10ms interval via `GCINTERVAL`. The workload repeatedly enqueues
objects and scans the queue, so shorter GC intervals keep the queue small and
reduce scanning overhead.
