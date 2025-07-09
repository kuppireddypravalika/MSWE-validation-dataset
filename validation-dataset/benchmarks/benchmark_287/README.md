# Benchmark 287: Avoid cloning call nodes in DSGraph merge

This benchmark is based on LLVM commit [2ea00e9](https://github.com/llvm/llvm-project/commit/2ea00e994e5d009429f31af66703f60b3c2a6850) by Chris Lattner. The commit adds flags `DSGraph::DontCloneCallNodes` and `DSGraph::DontCloneAuxCallNodes` when merging callee graphs during interprocedural mod/ref analysis.

By not cloning call nodes or auxiliary call nodes, the merge operation performs less work while producing the same analysis result. The microbenchmark models this behaviour using simplified DSGraph and DSCallSite classes and measures the cost of merging with and without these flags.
