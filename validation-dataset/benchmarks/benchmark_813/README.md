# Benchmark 813: hoist constants out of loop

This benchmark is derived from [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2) commit `ee07695c` which moved constant calculations outside a pixel filtering loop in `X8DrawingEngine::FilterRect`. The original version recomputed two expressions inside the nested loops, while the optimized version hoists them beforehand to avoid redundant work.

The benchmark replicates the drawing context and filter operation on a simplified buffer. It measures the effect of precomputing the scaled width of the rectangle and the step between rows.
