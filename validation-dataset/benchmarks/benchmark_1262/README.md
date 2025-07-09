# Benchmark 1262: Aligned memcpy optimization

This benchmark is derived from the Public Domain C Library (PDCLib) commit
[55a5c97](https://github.com/justincmiller/pdclib/commit/55a5c970340622f315856ab8698fc6e9d744a504)
which optimized `memcpy` by copying data eight bytes at a time when the source
and destination pointers share the same alignment. The original implementation
copied byte by byte regardless of alignment, while the patched version checks
alignment and performs wide copies to reduce iteration overhead.

The benchmark implements a simplified `my_memcpy` function reflecting this
change and measures copying large buffers repeatedly.
