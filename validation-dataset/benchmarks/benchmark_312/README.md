# Benchmark 312: SmallString concatenation in LLVM

This benchmark reproduces an optimization from LLVM TableGen where
string concatenation uses `SmallString` to avoid heap allocations.

- **Commit:** [1139977](https://github.com/llvm/llvm-project/commit/11399777f3770c72b8785c84b0921356c5ad6e35)
- **Author:** Matthias Braun
- **Optimization:** Use an on-stack `SmallString` instead of building a temporary `std::string`.
