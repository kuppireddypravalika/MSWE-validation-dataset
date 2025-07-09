# Benchmark 352: Force Interpreter vs JIT

This benchmark derives from LLVM commit [30411e0](https://github.com/llvm/llvm-project/commit/30411e0973fbad1bf304bdb3c1f8492ff61a935b) which switched the example *HowToUseJIT* program
from always forcing the interpreter to using the JIT engine. The change simply
passed `false` instead of `true` to `ExecutionEngine::create`, enabling JIT
execution once zero‑argument functions were supported.

The benchmark models this scenario with a tiny execution engine stub. When the
interpreter flag is `true` the engine repeatedly interprets the target function,
incurring heavy overhead. Passing `false` executes the function directly,
mirroring the JIT path.

The goal is to verify that using the JIT (`false`) noticeably improves
performance while producing the same result as the interpreter (`true`).
