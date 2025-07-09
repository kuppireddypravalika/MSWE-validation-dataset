# Benchmark 158: Ignore debug-only uses when emitting live-in copies

This benchmark is based on LLVM commit [7f995be](https://github.com/llvm/llvm-project/commit/7f995be32b31aa6f7d281ca4554a52caecb088a2) which changed `MachineRegisterInfo::EmitLiveInCopies` to use `use_nodbg_empty()` instead of `use_empty()`. The fix prevents debug value instructions from triggering unnecessary copy generation during code generation.

The simplified benchmark models a register info structure with many live-in registers that only have debug uses. The original implementation treats those registers as used and emits expensive copies, while the optimized version skips them.
