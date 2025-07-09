# Benchmark 641: fast floor on x86

This benchmark is based on [VTK](https://github.com/Kitware/VTK) commit `57b9ffec` which introduced a specialized implementation of `vtkMath::Floor` for x86 architectures. The new version uses a bit manipulation trick to compute the floor of a `double` more quickly than calling `std::floor`.

The benchmark measures the cost of repeatedly computing the floor of a value. The original implementation relies on the standard library `floor` while the optimized version uses the inline x86 specific routine.
