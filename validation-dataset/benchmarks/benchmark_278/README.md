# Benchmark 278: Avoid redundant base class offset lookup

This benchmark is based on the ROOT commit [9629ca8](https://github.com/root-project/root/commit/9629ca8e88a2f577240ed0e8abbc33b2710c92c7) which skips an expensive call to compute a base class offset when the base class is virtual. In the original implementation `GetDelta()` always queried `gCling` for the base class offset even though this lookup fails for virtual bases. The patch checks the `kIsVirtualBase` flag and immediately returns `-1` instead, avoiding the costly lookup.

The microbenchmark models this by simulating a slow `BaseClassInfo_Offset` call. The optimized version bypasses this call for virtual bases, mirroring the patch's logic.
