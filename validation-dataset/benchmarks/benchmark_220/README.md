# Benchmark 220: Using `memcpy` Instead of `std::copy_n`

This benchmark is extracted from the LLVM project. In [commit 9dc5489](https://github.com/llvm/llvm-project/commit/9dc548944127c89e25988a34df2884d6d5cbd14b), `std::copy_n` was replaced with `std::memcpy` when writing atom contents in the ELF writer. The change improves performance on MSVC where `std::copy_n` was slower than a direct `memcpy` call.

- **Commit:** [9dc5489](https://github.com/llvm/llvm-project/commit/9dc548944127c89e25988a34df2884d6d5cbd14b)
- **Author:** Michael J. Spencer
- **Optimization:** Replace `std::copy_n` with `std::memcpy` when copying atom content.

## Benchmark Goal
Evaluate whether replacing `std::copy_n` with `std::memcpy` speeds up copying large blocks of contiguous memory.
