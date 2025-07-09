# Benchmark 197: Use GetEntriesFast instead of GetEntries

This benchmark is derived from ROOT commit [6caa14a](https://github.com/root-project/root/commit/6caa14a36fb6d331d19f799782705b7661fb09bb) which replaced a call to `TTree::GetEntries()` with `GetEntriesFast()` in the constructor of `TTreeFilePrefetch`. The original implementation invoked `GetEntries()` which could trigger expensive `TChain::LoadTree` calls during construction. The optimized version uses `GetEntriesFast()` to avoid this recursive overhead.

The microbenchmark models a simplified tree and chain setup. `GetEntries()` performs a costly operation simulating the `LoadTree` call, while `GetEntriesFast()` simply returns a cached value. Constructing the prefetch object repeatedly highlights the performance difference between the two approaches.
