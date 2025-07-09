# Benchmark 396: Use staticEval in null prune condition

This benchmark is based on [commit ade3bb9](https://github.com/official-stockfish/Stockfish/commit/ade3bb9a4e774aa9b760235f3b0ee42e0a0420d9) from the Stockfish chess engine.

The patch modifies the null move pruning rule in the search routine.  When the depth is below 12 plies and the static evaluation is worse than `beta`, the engine no longer performs a costly null move search.  This avoids wasted computation on obviously bad lines.

The benchmark emulates the relevant control flow and measures the effect of the extra `staticEval` check.
