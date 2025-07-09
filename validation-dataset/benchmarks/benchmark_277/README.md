# Benchmark 277: Avoid vector reallocation when removing elements

This benchmark is based on LLVM lld commit [19237fb](https://github.com/llvm/llvm-project/commit/19237fb495f99d0a7b8a4554cef30851a0ae9228) which replaced copying a `std::vector` with an in-place `erase` to remove Windows resource files during linking.

The original implementation copied the remaining elements into a new vector after stable partitioning. The optimized version simply erases the unwanted range, avoiding an extra allocation and copy.

This microbenchmark models that behaviour using integer vectors.
