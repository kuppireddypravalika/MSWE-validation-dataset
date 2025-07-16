# Benchmark 2074: Avoid repeated strlen in drive loop

Derived from [Toucan](https://github.com/portableapps/toucan) commit `7cbd41e5` which moved the calculation of the drive string offset outside of a loop enumerating logical drives. The original implementation computed `strlen()` on each iteration, while the optimized version computes it once before entering the loop.

This benchmark models the drive enumeration logic to measure the impact of avoiding repeated length calculations.
