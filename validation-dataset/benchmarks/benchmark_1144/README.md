# Benchmark 1144: Dynamic scheduling for FBP2D loop

This benchmark is based on [commit bd09dea](https://github.com/UCL/STIR/commit/bd09deacd1bdd2c8ea928da681846f853541288e) from the STIR project. The patch modifies an OpenMP loop in `FBP2DReconstruction.cxx` by adding `schedule(dynamic)`. Some iterations of the loop are trivial because they correspond to non-basic symmetries, so using the default static schedule leads to load imbalance. Dynamic scheduling redistributes the iterations and improves performance.

The simplified benchmark emulates this behaviour with a loop over many "views" where most iterations do almost no work. The original version uses the default static scheduling, while the optimized version specifies dynamic scheduling.
