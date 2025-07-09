# Benchmark 374: Stop scanning redeclarations

This benchmark reproduces a performance fix from ROOT's clingutils module.
The original code iterated over all template redeclarations when searching for
an entry with default template arguments. The function
`getMinRequiredArguments()` is costly and the loop scanned the entire chain.
The patch breaks the loop upon finding the first redeclaration that provides
defaults.

- **Commit:** [34886d1](https://github.com/root-project/root/commit/34886d16d9ba956fde8bd853493abc9cba6fb42e)
- **Author:** Axel Naumann
- **Optimization:** Stop scanning after the first redeclaration with default
  template arguments.

This microbenchmark models a chain of redeclarations with an expensive
`getMinRequiredArguments()` call to highlight the runtime difference.
