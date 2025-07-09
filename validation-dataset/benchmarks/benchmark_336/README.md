# Benchmark 336: Use CreatePtrDiff to calculate pointer distance

This benchmark is based on [commit a0b54d9](https://github.com/leek-wars/leekscript-next/commit/a0b54d9635b36a07269b83b05b8ae4ea6b3acdfc) from the **LeekScript** project.
The patch replaced a manual computation of the index of an array iterator with
LLVM's `CreatePtrDiff` helper. The original code subtracted raw pointer values
and divided by the element size to obtain the index. The optimized version uses
native pointer difference, eliminating the division.

The benchmark models this behaviour with a small array of fixed-size structs.
The original implementation converts pointers to byte offsets and divides by a
run-time element size variable, while the optimized version directly subtracts
`Node*` pointers. The workload is tuned so the original function runs for more
than 5 seconds.
