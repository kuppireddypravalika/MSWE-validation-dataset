# Benchmark dataset_125: Skip serialization of empty modules

Derived from [rpcs3 commit d6cf167](https://github.com/RPCS3/rpcs3/commit/d6cf167d52b709854524b07f462790f90993de59) which optimized module serialization by skipping modules that contain no variables. The original implementation always serialized every module name and an empty list. The patch adds a check to continue the loop when the variable list is empty.

This benchmark constructs a list of modules with mostly empty variable lists and repeatedly serializes them. The optimized version avoids unnecessary writes for empty modules.
