# Dataset 215: Avoid repeated strlen in drive loop

Derived from [Toucan](https://github.com/portableapps/toucan) commit `7cbd41e5` which moved the calculation of the drive string offset outside of a loop enumerating logical drives. The original implementation called `strlen` for each drive, while the patched version computes the length once before the loop begins.

This benchmark models the drive enumeration logic to demonstrate the improvement of avoiding redundant length calculations.
