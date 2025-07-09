# Dataset 333: move AABB tree construction

This benchmark is derived from ResInsight commit `04311c64` which moved the construction of the axis-aligned bounding box (AABB) search tree outside a parallel loop in `RivSurfaceIntersectionGeometryGenerator::calculateArrays`. Building the tree inside the loop caused every thread to trigger the expensive initialization and could even crash. The patched version performs this initialization once before entering the `omp` region.

The benchmark models a grid class that lazily constructs an AABB tree the first time `findIntersectingCells` is called. In the original version each thread performs this construction separately. The optimized version performs the construction once before the parallel loop, so subsequent calls reuse the already built tree.
