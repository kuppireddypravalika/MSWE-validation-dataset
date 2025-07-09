# Benchmark 113: LLVM pass pipeline optimization

This benchmark derives from commit 65fc468 in the ATK project. The patch adds a validation step for the generated LLVM module and applies the default `O3` optimization pipeline using `PassBuilder` before creating the execution engine. Running these passes yields a faster dynamically generated function.
