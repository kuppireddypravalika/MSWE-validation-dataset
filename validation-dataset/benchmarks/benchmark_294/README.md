# Benchmark 294: dynamic_cast vs static_cast in vehicle simulator

This benchmark derives from a patch in the DUNE project ([a896ba1](https://github.com/LSTS/dune/commit/a896ba13ca6197f432498c4f9614a1529ecbd43f)) where a frequent `dynamic_cast` was replaced by `static_cast` inside the vehicle simulator. The code routinely casts a generic `Vehicle*` to a `UUV*` when processing `ServoPosition` messages. Because the object type is known, using `static_cast` avoids the runtime RTTI check of `dynamic_cast`.

Our simplified benchmark models this behaviour with minimal `Vehicle` and `UUV` classes. The original version performs the cast with `dynamic_cast` on every message, while the optimized version uses `static_cast`. The workload scales the number of messages to run for several seconds so the difference is measurable.
