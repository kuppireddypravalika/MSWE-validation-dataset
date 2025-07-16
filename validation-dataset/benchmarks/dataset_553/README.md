# Benchmark 2075: MP3 decoding loop improvement

Derived from [MusicPlayerDaemon commit 2a960853](https://github.com/MusicPlayerDaemon/MPD/commit/2a9608536c83373ae43c395176c5b41c5a5b5dec) which moved the `num_samples` calculation outside the inner loop in `mp3Read`. The original version recomputed the maximum chunk size every iteration, while the optimized version stores it once before the loop.

This benchmark models the decoding loop processing PCM samples from an output buffer. The optimized version avoids redundant division inside the loop.
