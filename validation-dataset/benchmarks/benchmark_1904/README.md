# Benchmark 1904: Vector multiply sum support check

This benchmark is based on OpenJ9/OMR commit [1e3b62e](https://github.com/eclipse/openj9/commit/1e3b62e082205711c447bd1c90698ca973100894) which added a check for vector register support before using the VMSL instruction in `inlineP256Multiply`. Without this check the JIT could attempt to emit vector instructions on systems without vector hardware, leading to slower emulation.

The benchmark models a code generator selecting between a slow emulated vector multiply and a faster scalar multiply. The original version unconditionally uses the vector routine when the processor architecture is `zNext`, even if vector registers are unavailable. The patched version additionally verifies vector register support.
