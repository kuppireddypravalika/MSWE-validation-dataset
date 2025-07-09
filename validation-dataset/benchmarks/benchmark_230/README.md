# Benchmark 230: Skip Parsing Function Bodies in Preamble

This benchmark is based on Qt Creator commit [8fa1589](https://code.qt.io/cgit/qt-creator/qt-creator.git/commit/?id=8fa15892a417bd4c2af33a9dab1f394e93539c54) which modified the default
libclang parse options used when creating a translation unit. The patch adds the
`CXTranslationUnit_SkipFunctionBodies` and `CXTranslationUnit_LimitSkipFunctionBodiesToPreamble`
flags when available, so that only the preamble of a source file is fully parsed.
Skipping heavy function bodies dramatically reduces the time and memory required
for reparsing.

Our benchmark simulates the parsing workload of many large function bodies.
The `original` version parses every token in the source text whereas the
`optimized` version skips over function bodies when those flags are enabled.
We measure the difference in total parsing time.
