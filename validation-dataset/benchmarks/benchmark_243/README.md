# Benchmark 243: QByteArray assign optimization

This benchmark is based on Qt commit [3bb488f](https://github.com/qt/qtbase/commit/3bb488fb2ac2e11602fe74dae35d074223d3192a) which changed the way `QByteArray` is filled in `QTlsBackendOpenSSL::dhParametersFromPem`. The new implementation uses `QByteArray::assign()` instead of copy-assigning from a temporary. This reuses existing capacity and avoids unnecessary allocations.

The benchmark compares the performance of assigning from a temporary string (simulating the old behaviour) versus using `assign()` on a preallocated buffer.

We model `QByteArray` using a lightweight `ByteArray` class that performs heap allocations on assignment. The original implementation assigns from a temporary instance, causing a new allocation each call. The optimized version uses `assign()` to reuse the existing buffer.
