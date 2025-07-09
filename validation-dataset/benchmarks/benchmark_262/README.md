# Benchmark 262: Avoid redundant byte swapping in Ogg decoder

This benchmark is derived from the Music Player Daemon (MPD) commit [`8d95265`](https://github.com/MusicPlayerDaemon/MPD/commit/8d95265cc8ba60ed8f51a07a6028f4a0dc56bf6d) which changes the `ogg_decode` routine to use the big-endian parameter of `ov_read` directly instead of manually swapping bytes with `pcm_changeBufferEndianness`.

- **Commit:** [`8d95265`](https://github.com/MusicPlayerDaemon/MPD/commit/8d95265cc8ba60ed8f51a07a6028f4a0dc56bf6d)
- **Author:** Warren Dukes
- **Optimization:** Let `ov_read` output data in the host endianness and remove an extra buffer conversion pass.

## Benchmark Goal

Replicate the performance impact of removing the separate endianness conversion step when decoding Ogg data on big-endian systems.

## Inefficiency Classification

- **Type:** Execution-time inefficiency
- **Reason:** The original implementation always read little-endian samples and performed an additional byte swap on big-endian builds. The patch forwards the big-endian flag to `ov_read` so that it outputs the correct layout directly, eliminating redundant work.
