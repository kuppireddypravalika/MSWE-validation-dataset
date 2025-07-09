# Benchmark 1369: image downsample AVX2 check

This benchmark is derived from commit `bd3f390b8e` in the **vfxGraph** project which corrected a preprocessor check from `__AVX` to `__AVX2` in the CPU image downsample node. The AVX implementation actually relied on AVX2 intrinsics, so the original code would either compile to a slower SSE path or fail when AVX2 support was missing. The optimized version explicitly uses AVX2 instructions when available.

The benchmark measures a 2×2 downsampling routine implemented with SSSE3 versus an AVX2 version.
