# Benchmark 132: IA-32 byteswap optimization

This benchmark originates from [SheepShaver](https://github.com/cebix/macemu) commit `9da81c79a2c6c91950b316fd89eeeac5b26f0d93`. The patch enables the `bswap` instruction for 32‑bit x86 builds and adds an optimized 64‑bit implementation using two 32‑bit swaps. The change replaces a generic byte swapping routine with inline assembly on IA‑32, speeding up endian conversions.

The microbenchmark reproduces this scenario by comparing a naive byte swap implementation (compiled with optimizations disabled) against an assembly version using the `bswap` instruction.
