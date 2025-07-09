# Benchmark 101: PHINode predecessor traversal optimization

This benchmark mirrors LLVM commit [9115eb3](https://github.com/llvm/llvm-project/commit/9115eb302457038788e9bbe1280b2fe31227b747) which adjusted the Aggressive Dead Code Elimination pass to iterate over `PHINode` incoming blocks directly instead of querying the parent `BasicBlock`'s predecessors. The change avoids unnecessary predecessor lookups when marking blocks alive.

- **Commit:** [9115eb3](https://github.com/llvm/llvm-project/commit/9115eb302457038788e9bbe1280b2fe31227b747)
- **Author:** Chris Lattner
- **Optimization:** Use `PHINode::getIncomingBlock` and `getNumIncomingValues` instead of `pred_begin` / `pred_end` when scanning live PHI nodes.

## Benchmark Goal

Demonstrate the performance difference between iterating over a basic block's predecessor list and directly using a PHI node's incoming blocks during liveness propagation.

## Optimization Type

- **Category:** Execution Time Inefficiency
- **Rationale:** The original implementation repeatedly queried the predecessor list for each PHI node. The optimized version accesses the already stored incoming blocks, reducing loop overhead.

