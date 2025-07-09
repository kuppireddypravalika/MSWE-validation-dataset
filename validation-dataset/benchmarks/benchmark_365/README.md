# Benchmark 365: Move semantics in LoadNode constructor

Derived from [ITK commit 76f27145c0](https://github.com/InsightSoftwareConsortium/ITK/commit/76f27145c07679f40b7bd9e1bc35459118804345).
The patch moves the force vector into the `LoadNode` member instead of copying it.
This benchmark constructs many `LoadNode` objects with large force vectors to
highlight the performance impact of the extra copy.
