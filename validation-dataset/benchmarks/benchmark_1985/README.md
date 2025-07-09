# Benchmark 1985: Eigen matrix rollup

This benchmark is derived from commit `23fca270` which optimized an RLC tank
simulation by replacing a manual loop computing the state derivative with a
single Eigen matrix expression. The original implementation iterated over the
rows, performing separate dot products. The optimized version maps the output
vector and evaluates the entire expression via matrix multiplication.
