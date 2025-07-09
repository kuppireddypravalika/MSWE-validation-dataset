# Benchmark 166: Strand Split Threshold Adjustment

This benchmark is based on Embree commit [4e8e73b](https://github.com/embree/embree/commit/4e8e73baea7148453607332a75a91c111c0f6471) which tuned a heuristic used during hair BVH building. The optimization raised the threshold for attempting expensive strand splitting from `0.6` to `0.7` times the leaf SAH. By skipping the costly search more often, build times improved by about 25% while rendering quality remained unaffected.

The simplified benchmark replicates this behavior. The original implementation tests the lower `0.6` factor and therefore performs the heavy split operation for more primitives. The optimized version uses `0.7`, reducing the number of heavy calls. The output of both versions is identical because the expensive computation only affects performance.
