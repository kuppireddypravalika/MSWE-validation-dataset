# Benchmark 176: accelerate empty cluster removal

This benchmark is derived from PCL commit `bea7f5a7` which optimized
`RegionGrowingRGB::assembleRegions` by replacing repeated `erase` calls
on the cluster vector with a two-pointer swap approach. The original
version iterated through the vector and erased empty segments one by
one, causing quadratic behavior when many segments were empty. The new
implementation swaps non-empty segments to the front and erases the
trailing block in a single step.

The benchmark constructs a simplified scenario with many clusters where
around one third are empty. We repeatedly call `assembleRegions` to
measure the time spent removing empty clusters.
