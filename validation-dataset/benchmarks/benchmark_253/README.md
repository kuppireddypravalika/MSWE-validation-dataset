# Benchmark 253: Qt path reconstruction optimization

This benchmark models a fix from KDE Baloo ([commit 5d1bcc4](https://invent.kde.org/kde/baloo/-/commit/5d1bcc417e14df088d75c7449edca385524cfdc6)) where the document URL database rebuilt file paths.
The original implementation accumulated every path segment in a `QByteArrayList` and finally called `join('/')`. The optimized code constructs the
path incrementally in a single `QByteArray` which reduces allocations and memory usage.

Our simplified benchmark mimics this behaviour with a small in-memory database. The `original` version uses a `std::vector<std::string>` and joins
at the end, while the `optimized` version concatenates directly during traversal. The workload is scaled so the original version runs for over 10s on
the reference hardware.
