# Benchmark 2290: Reduce realloc frequency when reading ISO8211 records

This benchmark is derived from [GDAL](https://github.com/OSGeo/gdal) commit `e2d0dce7`
which changes the logic in `DDFRecord::ReadHeader()` to avoid calling `realloc()`
on each additional byte read when a data record is longer than expected.

The original implementation extended the buffer by one byte at a time, causing
many reallocations if the terminator was far from the predicted record length.
The patched version doubles the allocation size whenever the buffer needs to
grow, greatly reducing the number of reallocations.

The benchmark simulates reading a record from a memory file that requires many
extra bytes before the field terminator appears. It measures the cost of the
reallocation strategy in the original and optimized versions.
