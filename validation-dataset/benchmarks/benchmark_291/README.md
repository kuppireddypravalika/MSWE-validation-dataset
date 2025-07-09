# Benchmark 291: Eliminating Q_FOREACH Copy Overhead

This benchmark is based on Qt commit [db27f3d](https://github.com/qt/qtbase/commit/db27f3d5a7b5ea3e415b3985877c8951f8eabfbb) which replaced a `Q_FOREACH` loop with a C++11 range-for loop in `QLocalSocket::setSocketDescriptor`. The `children()` method returns a container by const reference, so using `Q_FOREACH` causes an unnecessary copy of that container on each invocation.

The optimized version iterates directly over the container via a range-for loop, avoiding the copy.

## Benchmark Goal

Replicate the performance difference caused by iterating over a copy versus iterating by reference when scanning a list of child sockets.

## Optimization Type

- **Category:** Loop Inefficiency / Unnecessary Copy
- **Rationale:** `Q_FOREACH` copies the container before iteration. Replacing it with a range-for loop removes this copy, reducing memory allocations and iterations.
