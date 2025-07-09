# Benchmark 2074: convert to time_t once

This benchmark is based on Ceetron Solutions commit [fbd8fff](https://github.com/OPM/ResInsight/commit/fbd8fffcca2bba25a66dd7ef385436322669e21c) which optimizes `extractValueForTimeStep` in `RimSimWellInViewTools`.

The original implementation converted each resampled timestamp to a `QDateTime` object inside a loop before comparing it with the current date. The patch avoids this repeated conversion by converting the current date to `time_t` once and comparing the raw `time_t` values directly.

The benchmark models a vector of timestamps that are repeatedly scanned to find the first entry after a given date. The optimized version performs the comparison using the precomputed `time_t` value, eliminating costly conversions on every iteration.
