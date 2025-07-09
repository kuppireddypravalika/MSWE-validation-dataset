# Benchmark 388: DerivedType dropAllTypeUses micro-optimization

This benchmark is based on LLVM commit [4338f905b79bf996bfa10ba515f915e2986bcccb](https://github.com/llvm/llvm-project/commit/4338f905b79bf996bfa10ba515f915e2986bcccb) which cached a pointer to `Int32Ty` inside `DerivedType::dropAllTypeUses` to avoid redundant lookups.

The patch reduces overhead when clearing contained type references during type refinement.
