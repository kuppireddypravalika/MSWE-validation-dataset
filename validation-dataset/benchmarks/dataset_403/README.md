# Dataset 403: MapViewState waste processing loop

Derived from [OutpostHD](https://github.com/ldicker83/OPHD) commit `2a39dc8f` which refactored the waste processing loop in `MapViewState::updateBiowasteRecycling`.
The original code iterated over all recycling facilities even after all residences were processed and used fixed constants for the per-facility limits. The optimized version breaks early once all residences have been handled and uses facility specific values for waste processing capacity and supported residence count.

This benchmark constructs vectors of mock `Residence` and `Recycling` structures and measures the time spent processing them using the original and optimized logic.
