# Benchmark 879: cache time delta outside loop

Based on commit [72997c5](https://github.com/jeremyevans/aether/commit/72997c5f7af3245e84b66fb2163a5502967052c2)
which optimized `Aether::UI::View::update_peaks`. The original code queried
`steady_clock::now()` for every peak to compute a time delta. The patch computes
this delta once before the loop, removing redundant calls.

This benchmark models a `View` with many peak values updated every frame. The
harness repeats the update to measure the overhead of repeatedly querying the
clock.
