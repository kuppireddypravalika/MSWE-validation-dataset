# Benchmark 1834: Jacobian product using vnl_fastops::AtA

Derived from ITK commit [`228407c`](https://github.com/InsightSoftwareConsortium/ITK/commit/228407c45c88fe9fdba242e37ac5ecdd06827a0d)
which replaced a manual matrix multiplication `M = (*Jacobian) * Jacobian->transpose()`
with the optimized helper `vnl_fastops::AtA(*Jacobian, &M)`.
The helper avoids redundant work when forming `J * J^T`.

This benchmark models the computation of a 6xN Jacobian matrix product.
The original implementation performs a full matrix multiplication while the
optimized version exploits symmetry to halve the inner loop work.
