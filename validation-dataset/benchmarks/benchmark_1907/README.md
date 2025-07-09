# Benchmark 1907: MOVSHDUP simplification

Derived from [FEX](https://github.com/FEX-Emu/FEX) commit `e0e6b3ad6b` which simplified the implementation of the SSE `MOVSHDUP` instruction. The original version inserted four elements into a newly allocated IR node, while the optimized version only inserts two elements, reducing node allocations.

This benchmark models the IR builder behavior by allocating a small `OrderedNode` structure for each insertion. The performance harness repeatedly performs the operation millions of times to highlight the reduction in allocations.
