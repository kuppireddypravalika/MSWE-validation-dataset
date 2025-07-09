# Benchmark 896: uilog2 intrinsic optimization

This benchmark is based on [commit 214c3599](https://github.com/gerbilsoft/rom-properties/commit/214c359922da6944a42a59fa2405be631baf9a16) from the `rom-properties` project. The change replaces a loop used to compute the integer base-2 logarithm with compiler intrinsics (`__builtin_ctz` or `_BitScanReverse`).

The original version iterates shifting the input until it becomes zero. The optimized version relies on a single intrinsic call, significantly reducing the work per invocation.
