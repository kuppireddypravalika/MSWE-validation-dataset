# Benchmark 331: isGlobal check for slot lookup

This benchmark is based on Firefox's TraceMonkey commit [9b9d605](https://github.com/mozilla/gecko-dev/commit/9b9d60583cca235bebe13f6db23e0b60b0c5ba4a) by Andreas Gal.

The original implementation determined whether a pointer referred to a global slot by scanning the tracked `globalSlots` table for every store. The patch introduces a fast `isGlobal()` check that quickly detects non-global pointers, avoiding the linear scan and only performing the lookup when needed.

The benchmark simulates repeated writes to stack locations while a large table of potential global slots is present. The optimized version skips scanning this table when writing to non-global addresses, mirroring the behavior of the patch.
