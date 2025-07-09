# Benchmark 311: Compiler label branching

This benchmark is derived from [commit 57ae828](https://github.com/?/commit/57ae828750ee32eed3a04492a3bbd93f3ea99c68) which replaces direct usage of low level `jit_label_t` and `jit_insn_branch_if` with the higher level `Compiler::label` wrapper in `pow_mpz_int`.

The change avoids the overhead of manipulating raw JIT labels and exposes a simpler API. We measure the effect by implementing simplified versions of both approaches.
