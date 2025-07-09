# Benchmark 363: Locale-free strtod replacement

This benchmark is based on [OpenHRP3](https://github.com/fkanehiro/openhrp3) commit `c4d7fa2`,
which replaced calls to the standard `strtod` with a custom parser. The change
avoids locale-dependent behaviour and improves parsing performance.

The benchmark repeatedly parses a space-separated list of floating point values.
The original implementation uses `strtod` while the optimized version uses the
custom `mystrtod` routine from the commit.
