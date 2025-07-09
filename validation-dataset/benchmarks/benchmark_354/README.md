# Benchmark 354: is64BitProcessor check simplification

This benchmark is based on OpenJ9 commit [df76d09](https://github.com/eclipse-openj9/openj9/commit/df76d092295bcc1a4aa3d7aa85e0abf70e091f0a) which optimized the `is64BitProcessor` method.
The original implementation always queried CPU properties at runtime to determine
whether the processor supported 64‑bit instructions. The patch short-circuits
this check when the target build is already 64‑bit, avoiding the expensive
runtime query.

The benchmark models this behaviour with a minimal `CodeGenerator` class and a
mock CPU query that performs work to simulate the cost of inspecting the system.
We repeatedly invoke `is64BitProcessor` to highlight the performance difference
between the two implementations.
