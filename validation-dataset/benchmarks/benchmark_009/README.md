# Benchmark 009: Avoiding Double-to-Float Conversion in Octree Search

This benchmark is based on the PCL commit [7abcd66](https://github.com/PointCloudLibrary/pcl/commit/7abcd66865ca2a382d7d8eca0092a9584a7083b1), which replaces `double` with `float` in nearest neighbor search distance tracking to avoid unnecessary type promotion.

- **Optimization:** Avoid implicit conversion from float to double.
- **Expected Benefit:** Slight performance gain and better alignment with function return types (`pointSquaredDist` returns `float`).
- **Commit Author:** Markus Vieth

## Benchmark Description

Simulates a brute-force nearest neighbor search in a 3D point cloud using float distances. Measures performance before and after replacing `double` with `float`.

## Inefficiency Classification

- **Type:** Execution-time inefficiency
- **Reason:** The original implementation used `double` to store squared distances even though the distance computation function (`pointSquaredDist`) returned `float`. This mismatch caused unnecessary type promotion and redundant precision, which added overhead in tight-loop nearest neighbor computations.
