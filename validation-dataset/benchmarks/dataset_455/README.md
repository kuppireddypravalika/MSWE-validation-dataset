# Benchmark 2455: SongFilter Parse loop simplification

This benchmark is derived from the Music Player Daemon commit
[`b34fd90`](https://github.com/MusicPlayerDaemon/MPD/commit/b34fd905c8f7ce29cd1d2acae5d90fa8ad8b05ed)
which rewrote the `SongFilter::Parse(ConstBuffer)` loop to consume the
buffer using `shift()` instead of indexing. The change enables easier
extension of the parser and slightly reduces overhead of pointer
arithmetic.

The benchmark models a minimal `SongFilter` class and `ConstBuffer`
container. The original version iterates the buffer with an index
variable, while the optimized version repeatedly calls `shift()` to
consume elements.
