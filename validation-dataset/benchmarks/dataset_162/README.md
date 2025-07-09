# Benchmark 162: for() efficiency in Qalculate

Derived from [Qalculate](https://github.com/Qalculate/libqalculate) commit `9b9e4462`.
The patch avoids calling the costly `calculatesub()` method for every loop
iteration in the `for()` built‑in function. The optimized version initially uses
`eval()` and only falls back to `calculatesub()` when the result contains
additions or functions.
