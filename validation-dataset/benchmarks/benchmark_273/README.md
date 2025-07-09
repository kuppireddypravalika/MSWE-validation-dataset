# Benchmark 273: Clip mask optimization

This benchmark is based on [Mozilla commit a79e2ae](https://github.com/mozilla/mozilla-central/commit/a79e2ae980718ee1a1f742e92f884cca38621755) which adjusts clipping logic in `cairo-d2d-surface.cpp`.
The original code always processed clip boxes even when the mask was not a solid alpha mask, doing needless work.
The patched version restricts this optimization to solid alpha masks only.

Our simplified benchmark replicates this by calling a costly `heavy_clip_get_boxes` function whenever a clip is present.
The optimized variant skips this call for non-solid masks, mirroring the upstream change.
