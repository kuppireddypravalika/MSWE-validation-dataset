# Benchmark 285: CloneTree options in TTree::MergeTrees

This benchmark is based on the ROOT commit [f8fc806](https://github.com/root-project/root/commit/f8fc80646254b7087df03c78662dea1ae87fb4a9) which modifies `TTree::MergeTrees`.

- **Optimization:** Pass the merge options when cloning the first tree so that fast cloning can be enabled.
- **Expected Benefit:** Allows fast cloning of the first tree when the caller requests it via the options string.
- **Commit Author:** Philippe Canal

## Benchmark Description

The benchmark emulates merging multiple trees. The original implementation ignored the options when cloning the first tree which disabled fast cloning. The optimized version passes the options to `CloneTree` enabling the fast path.

## Inefficiency Classification

- **Type:** Execution-time inefficiency
- **Reason:** Failing to pass the fast-cloning option prevented an optimized code path from running.
