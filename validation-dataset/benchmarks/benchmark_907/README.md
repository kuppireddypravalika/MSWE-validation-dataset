# Benchmark 907: LiveInterval operand loop speedup

This benchmark is derived from LLVM commit [774785a](https://github.com/llvm/llvm-project/commit/774785a79d14c95d467dd0224761723d51b5f9da).
The patch introduced a minor optimization in `LiveIntervalAnalysis::runOnMachineFunction` by caching the number of operands in a local variable when iterating over a `MachineInstr`'s operands. This avoids repeatedly calling `getNumOperands()` inside the loop.

The benchmark models a simplified version of the LLVM data structures involved. It repeatedly walks over many `MachineBasicBlock` objects and updates interval weights for each operand. The original implementation calls `getNumOperands()` on every iteration of the inner loop while the optimized version stores the count once.
