# Benchmark 342: OpenMP schedule optimization

This benchmark is derived from the Pinocchio robotics library where the collision
computation kernel changed its OpenMP directive from `schedule(dynamic)` to
`schedule(static)`.

- **Commit:** [29fbdeb](https://github.com/stack-of-tasks/pinocchio/commit/29fbdeb7c2352737e622a6eb73dcd0e1e678ccb2)
- **Author:** Justin Carpentier
- **Optimization:** Use a static schedule for parallel loops to reduce OpenMP scheduling overhead.

## Benchmark Goal
Show the performance impact of switching an OpenMP loop from dynamic to static scheduling.
