# Benchmark 149: LICM hoist moveBefore optimization

This benchmark is based on LLVM commit [bb07ed3](https://github.com/llvm/llvm-project/commit/bb07ed3bb5db1b610362628ceb31349667c491c2) by Chris Lattner.
The patch optimizes `LICM::hoist` by using `Instruction::moveBefore` instead of
removing and reinserting the instruction and by updating the alias set tracker
when the instruction leaves the loop.

The benchmark models the hoisting of many instructions from a loop preheader.
The `original` version performs a removal followed by an insertion, while the
`optimized` version splices the node directly and updates a mock alias set
tracker.

