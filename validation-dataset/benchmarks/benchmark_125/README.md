# Benchmark 125: Avoid unnecessary else label

This benchmark is derived from commit [e2d7721](https://github.com/?/commit/e2d772129efa5d8cc1b8d9fa9f93524aedb741ed) which optimizes `Compiler::insn_if`. The original implementation always created an `else` label using `llvm::BasicBlock::Create` even when no `else` block was present. The patch skips this allocation when the `else` callback is absent, avoiding extra BasicBlock creation.

The benchmark measures the cost of repeatedly invoking `insn_if` without an else branch.
