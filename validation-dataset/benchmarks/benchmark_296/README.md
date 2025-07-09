# Benchmark 296: Skip non transition OSR slots

Derived from [OpenJ9 commit ee5f10f](https://github.com/eclipse-openj9/openj9/commit/ee5f10f94488338dbdae67d126362291c7f86f94) which avoids collecting
OSR slot sharing information for instructions that will never cause a
transition under voluntary OSR. The change introduces an early check in
`TR_OSRCompilationData::addInstruction` that bypasses expensive slot
processing for most instructions.

This benchmark models the original compile time behaviour with a simplified
`TR_OSRCompilationData` class. The original version processes every
instruction while the optimized version skips those without the special
`TR_induceOSRAtCurrentPC` symbol reference when running in voluntary mode.
