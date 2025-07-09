# Benchmark 1099: delayed intersection distance

This benchmark is based on [Embree](https://github.com/embree/embree) commit `55c9bffc` which rearranged the ray–triangle
intersection code in `triangle1_intersector16_moeller.h`. The original version computed the intersection
parameter `t` before checking whether the barycentric coordinates were valid. The patch
moves this computation after the early-exit check so that the expensive dot product is
avoided when the ray misses the triangle.

The benchmark constructs a simple set of triangles and repeatedly performs intersection
tests. The `original` implementation computes `t` unconditionally, while the `optimized`
implementation only computes it if the barycentric checks succeed. Both versions
produce identical results but the optimized version avoids unnecessary work when
most rays miss.

For reference the directory also contains the original and patched headers from
Embree (`kernels_xeon_geometry_triangle1_intersector16_moeller.h.before` and
`kernels_xeon_geometry_triangle1_intersector16_moeller.h.after`) as well as the
patch file `55c9bffc18a897e14d7aea54314924960dc968dd.patch`.

