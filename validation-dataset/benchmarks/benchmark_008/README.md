
# Benchmark 008: Efficient Median Calculation

This benchmark evaluates the performance improvement achieved by replacing `std::sort` with `std::nth_element` for median calculation.

- **Commit:** [a8defb0](https://github.com/PointCloudLibrary/pcl/commit/a8defb0250502e06150dab34b76866dacf635f49)
- **Author:** Markus Vieth
- **Optimization:** Replaced full sort with partial sort using `std::nth_element` to compute the median more efficiently.

## Benchmark Goal

Assess the performance benefits of using `std::nth_element` over `std::sort` for median computation in large datasets.
