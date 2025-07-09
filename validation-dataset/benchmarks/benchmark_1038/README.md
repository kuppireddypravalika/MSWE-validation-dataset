# Benchmark 1038: split ql calculation loops

Derived from [microhh](https://github.com/vanstratum/microhh) commit `235807c6` which separated the non-vectorizable `calcql` iteration in `cthermo_moist::calcbuoyancy` into its own loop. The original version computed a first estimate of liquid water and immediately performed the expensive saturation adjustment inside the same loop. The optimized version first stores the estimate, then runs a second loop applying `calcql` only where needed, improving vectorization.
