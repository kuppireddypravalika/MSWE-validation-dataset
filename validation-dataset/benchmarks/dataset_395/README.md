# Benchmark dataset_395: preallocate curve values

This benchmark is based on ResInsight commit `ea87c84` which modified `RifColumnBasedUserData::values` to preallocate the destination vector before copying curve values. The change avoids repeated reallocations when many values are copied.
