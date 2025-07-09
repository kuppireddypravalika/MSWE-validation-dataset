# Benchmark 292: Remove redundant upvalue stack operations

This benchmark is based on commit [9548494](https://github.com/matthew-spraggs/loxx/commit/954849438d371c1fcdccf6d72e3d1ca35e0e29d8) where
extra `locals_` and `upvalues_` stack pushes/pops were removed when compiling
classes with a superclass. The original implementation created temporary
empty vectors each time a `super` variable was declared, incurring unnecessary
allocation overhead. The optimized version simply begins and ends the scope
without modifying these stacks.

Our microbenchmark replicates this behavior with simplified data structures.
The `original` version pushes and pops large vectors on every class
compilation while the `optimized` version omits these operations. The workload
is scaled to ensure the difference is observable.
