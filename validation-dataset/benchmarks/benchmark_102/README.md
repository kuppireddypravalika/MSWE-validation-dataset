# Benchmark 102: Remove large stack allocation in entry frame

This benchmark is based on OpenJDK commit [72af507](https://github.com/openjdk/jdk/commit/72af50709932f24a4492ce74fb4c8ec9c04dc84f) which modified the AArch64 stub generator. The original code always subtracted an entire memory page from the stack pointer when preparing to call into Java, regardless of the actual number of arguments. The patch changed this to allocate only the space required for the arguments and align the stack pointer properly, avoiding unnecessary stack usage.

The benchmark models this behaviour with a small function that copies a list of
parameters onto stack memory before calling a dummy routine. The `original`
version allocates a full 4KB page on each invocation while the `optimized`
version allocates only the bytes needed for the parameters.

The loop count in the harness is tuned so that the original implementation runs
for over **10 seconds** even on fast machines. This keeps the performance
difference visible when validated in CI.

