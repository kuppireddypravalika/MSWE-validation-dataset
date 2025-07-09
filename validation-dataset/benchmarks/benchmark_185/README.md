# Benchmark 185: Prune dangerous moves at low depth

This benchmark is based on Stockfish commit [38428ad](https://github.com/official-stockfish/Stockfish/commit/38428ada5459903c4e68130751478d66f90b7c6a) which added an early pruning check in the search loop. The change skips moves that are captures, promotions, checks or advanced pawn pushes when their static exchange evaluation is negative at very shallow depth.

The optimized version performs this check before the expensive move evaluation whereas the original performs the same check only afterwards. The overall search result is unaffected but a significant amount of work is avoided.
