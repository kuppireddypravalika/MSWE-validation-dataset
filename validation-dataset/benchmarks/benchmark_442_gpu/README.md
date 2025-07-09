# Benchmark 442 GPU: dynamic scheduling with atomic updates

This benchmark is derived from [SG++](https://github.com/SGpp/SGpp) commit `1159c81b` which updated `OperationMatrixLTwoDotNakBsplineBoundaryCombigrid::mult`. The patch switched the OpenMP loop from static to dynamic scheduling and added `#pragma omp atomic` around shared result updates. This enables parallel execution without races when using dynamic scheduling.
