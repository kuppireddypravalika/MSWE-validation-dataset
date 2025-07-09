# Benchmark 293: move strings in config

This benchmark is based on [commit 933cdf7](https://github.com/FEX-Emu/FEX/commit/933cdf76b41b6f2ebda8d1b8e4bff1bc5f68f45f) from the FEX emulator project.
The patch changed several configuration helpers to move `std::string` values
instead of copying them. It also rewrote `EraseSet` to call `Erase` and `Set`.
The goal was to reduce allocation churn when repeatedly updating configuration
options.

This benchmark replicates the original and optimized implementations of
`Set`, `EraseSet` and the `Value` constructor, showing the effect of using
`std::move` on large temporary strings.
