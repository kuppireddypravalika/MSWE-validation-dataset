# Benchmark 221: Dynamic cast vs direct pointer

This benchmark is based on Embree commit [67ef201b](https://github.com/embree/embree/commit/67ef201b4e43175cb58dff72249892c7f65b0f32) which replaced costly `dynamicCast` calls with direct pointer casts when accessing geometry objects during collision detection.

The patch avoids creating temporary reference objects and performing runtime type checks for every triangle pair tested, improving performance of the hot loop.
