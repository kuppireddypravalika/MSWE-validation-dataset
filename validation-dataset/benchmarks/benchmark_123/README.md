# Benchmark 123: Remove unnecessary std::string copies

This benchmark is based on [LLVM commit bab934a](https://github.com/llvm/llvm-project/commit/bab934a4942923606c816962096fbcfcf7d41161) which avoided constructing a temporary `std::string` when passing a `SmallString` to Apple crash reporting APIs. The change simply forwards the internal buffer using `c_str()` instead of creating an extra `std::string` object.

The microbenchmark replicates the control flow of the original `CrashHandler` implementation. The `original_handler` builds a `SmallString`, then converts it to a temporary `std::string` twice before calling mock APIs. The `optimized_handler` uses `c_str()` directly. The benchmark repeatedly calls these handlers to measure the impact of the removed copies.

