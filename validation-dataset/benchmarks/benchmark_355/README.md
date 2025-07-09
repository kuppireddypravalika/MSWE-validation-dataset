# Benchmark 355: Avoid redundant PLT fix-ups

This benchmark is derived from LLVM's mclinker commit [310eefc](https://github.com/llvm-mirror/mclinker/commit/310eefcec21d54ac779dd0fcf873a6f2ae72545a).
The patch moved the calls to `applyPLT0` and `applyPLT1` inside `emitSectionData`
so they are executed only once when emitting the Procedure Linkage Table instead of
for every entry.

Our simplified benchmark models a `MockPLT` structure that requires these
fix-ups before its contents are copied into a buffer. The `original` version
invokes the fix-up functions for each entry during copying, while the
`optimized` version performs them once up front.

The harness repeats the emission routine many times to ensure the performance
difference is measurable (the original runs for over 10 seconds in CI).
