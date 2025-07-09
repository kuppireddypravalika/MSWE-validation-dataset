# Benchmark 341: Remove unnecessary std::move

This benchmark reproduces the performance impact of returning a local object with `std::move` which disables Named Return Value Optimization (NRVO). The optimization simply returns the local variable directly to enable NRVO.

- **Commit:** [395e333](https://github.com/KDE/kmymoney/commit/395e333ee135046edf3ad92119f4c82bc79e2518)
- **Author:** Thomas Baumgart
- **Optimization:** Avoid `std::move` on return to allow NRVO.

## Benchmark Goal

Measure the overhead introduced by `std::move` on a large object when returning by value.

## Optimization Type
- **Category:** Memory Operation Optimization
- **Rationale:** Copy elision avoids moving a large structure, reducing memory traffic.
