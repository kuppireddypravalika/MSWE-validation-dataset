# Benchmark 301: optional::emplace optimization

This benchmark is based on [commit e999e7c](https://github.com/tristbrindle/flux/commit/e999e7c5d7ac7447bc1fd8244af9733e58d0e0b8) from the Flux library.
The patch replaced assignments of `std::optional` temporaries with direct
`emplace()` calls in the `flatten` adaptor. Constructing the value in place
avoids an unnecessary temporary and reduces copies during iteration.

The benchmark replicates the single‑pass `flatten` logic using vectors of
vectors. The original version assigns a newly constructed `optional` each time
an inner sequence is read. The optimized version uses `emplace` to construct the
value directly in the existing `optional`.
