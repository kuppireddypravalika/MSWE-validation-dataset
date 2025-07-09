# Benchmark 224: Skip Windows extended ICU data

This benchmark is derived from [ICU commit cd78e6a03a342f6d618cefbd37dea1dec4f537d4](https://github.com/unicode-org/icu/commit/cd78e6a03a342f6d618cefbd37dea1dec4f537d4).
The patch introduces a Windows-specific short-circuit in `extendICUData()` that
avoids loading an additional versioned data file. By returning immediately when
`ICU_DATA_DIR_WINDOWS` is defined, the function skips file I/O and mutex work,
saving runtime in the Windows build.
