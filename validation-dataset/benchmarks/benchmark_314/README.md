# Benchmark 314: Extend Unsafe CAS intrinsics acceleration check

This benchmark is derived from [commit 3541ed5](https://github.com/eclipse-openj9/openj9/commit/3541ed5199f2a55da56d4e3dcd5cccfe355db54e) of the OpenJ9 project. The patch updated `callDoesAnImplicitAsyncCheck` so that certain `Unsafe.compareAndExchange*` intrinsics are recognized as having implicit asynchronous checks on IBM Z CPUs. Without this change the JIT inserted redundant checks, slowing down code generation.

The benchmark models the logic of `callDoesAnImplicitAsyncCheck` and the expensive work triggered when the check returns `true`. The optimized version adds the `isZ()` test so the heavy work is skipped on Z just like the real patch.
