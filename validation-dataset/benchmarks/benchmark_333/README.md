# Benchmark 333: Use tMOVgpr2gpr instead of t2MOVr

This benchmark is based on LLVM commit [052053b](https://github.com/llvm/llvm-project/commit/052053bbe3169a3574cb5af026cf0a5d616ae04d) which switched the epilogue code in `ARMBaseRegisterInfo::emitEpilogue` to emit the Thumb instruction `tMOVgpr2gpr` rather than `t2MOVr` when restoring the stack pointer from the frame pointer on Thumb2 targets.

The original implementation emitted the 32‑bit `t2MOVr` instruction with several additional operands. The patch replaced it with the lighter weight `tMOVgpr2gpr` instruction, reducing the work performed during code generation.

This benchmark replicates the control flow and conditional logic around this instruction selection to measure the performance impact of using the cheaper Thumb instruction.
