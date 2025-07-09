# Benchmark 104: Moving shared_ptr to avoid copy

This benchmark derives from [GDAL commit d9c631e9](https://github.com/OSGeo/gdal/commit/d9c631e9d8ed1df067dbfc989fd5f609428b3494) which addressed a performance warning.
In `TileDBDataset::Open`, the selected `std::shared_ptr<GDALMDArray>` was copied
on assignment. The patch replaces the copy with `std::move` so the ownership is
transferred without incrementing the reference count.

Our minimal reproduction models the same control flow: iterating over array
names, opening each as a `shared_ptr`, and storing the first 2D XY array as the
candidate. Copying this pointer millions of times is noticeably slower than
moving it.

