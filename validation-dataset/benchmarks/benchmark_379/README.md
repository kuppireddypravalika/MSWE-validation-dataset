# Benchmark 379: GetProtoNorm optimization

This benchmark originates from the ROOT project commit [733d892](https://github.com/root-project/root/commit/733d892ef6f157c55c28f98e7b39c018cff1e6eb). The patch updates `DeepAutoLoadImpl` to use `TClassTable::GetProtoNorm()` instead of `GetProto()` because the passed class name is already normalized.

The change avoids repeated normalization of class names during deep autoloading, speeding up traversal of class members.

The synthetic benchmark recreates the recursive lookup of proto classes. The original version normalizes each name on every call, while the optimized version looks up the already-normalized string.
