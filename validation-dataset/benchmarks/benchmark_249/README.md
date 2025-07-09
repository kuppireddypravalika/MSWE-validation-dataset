# Benchmark 249: Use Ray::invDirection

This benchmark originates from [TrinityCore](https://github.com/TrinityCore/TrinityCore) commit [428d8dc](https://github.com/TrinityCore/TrinityCore/commit/428d8dc3b7616f7bb9d39c4b6343c5eec4e3c965) where the bounding interval hierarchy ray intersection function stopped recalculating the inverse ray direction and instead reuses the precomputed value stored in `G3D::Ray`.

The original implementation allocated a temporary vector and computed `1.f / dir[i]` for each axis inside the intersection routine. The optimized version references `Ray::invDirection()` directly, avoiding three divisions per call.

The benchmark mirrors this behaviour with a lightweight `Ray` and `AABox` implementation. It repeatedly performs a bounding box intersection check using both the original and optimized methods.
