# Benchmark 742

This benchmark is derived from [rom-properties](https://github.com/GerbilSoft/rom-properties) commit `52e758b8`.
The patch optimizes the check for blank data in the Nintendo DS secure area.
The original implementation scanned a `uint32_t` array using reverse indexing.
The optimized version uses `uintptr_t` and reads two elements per iteration via
pointer arithmetic, enabling 64-bit comparisons on 64-bit systems.

