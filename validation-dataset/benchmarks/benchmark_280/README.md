# Benchmark 280: Extern Call Argument Flattening

This benchmark originates from Halide commit [adcb095](https://github.com/halide/Halide/commit/adcb09530395f067b1f95eaf22ba709b68c75abf) by Andrew Adams.
The patch fixed a bug in the `FlattenDimensions` lowering pass where the mutator failed
to recurse into the arguments of external function calls. As a result, loads
inside those arguments were not flattened which incurred unnecessary overhead.
The fix recursively mutates each argument and reconstructs the call only when
changes occur.

The benchmark models this behaviour with a simplified IR. The `original` version
leaves expensive two‑dimensional loads inside an extern call, while the
`optimized` version replaces them with flattened one‑dimensional loads.

