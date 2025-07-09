# Benchmark 144: Remove unnecessary FFmpeg flag

This benchmark is based on Kodi's commit [f4fadb3](https://github.com/xbmc/xbmc/commit/f4fadb3ba4583c45fb06908a3eb352be8c29f235) which removed usage of `AVFMT_FLAG_PRIV_OPT` when opening a media file with FFmpeg. Clearing and reapplying this flag around `avformat_open_input()` incurred extra work without providing benefits.

Our microbenchmark models the flag toggling behaviour. The `original` implementation sets and clears a flag before two expensive open operations while the `optimized` version performs the same opens without modifying the flag. The workload emulates the heavy initialisation that FFmpeg performs, so removing the flag reduces the amount of work.
