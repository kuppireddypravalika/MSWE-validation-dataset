# Benchmark 391: Cache Attached Property Lookup

This benchmark reproduces a performance optimization in Qt's `qmlAttachedPropertiesObject` helper.
The original implementation looked up the function pointer for each call using
`qmlAttachedPropertiesFunction`, which scans a registry based on the object's meta type.
By caching this function pointer as a static value inside the templated helper, subsequent
lookups avoid the linear search.

The benchmark simulates a registry of many types and repeatedly queries the attached
properties object for the same type.

- **Commit:** [1ee7afa](https://github.com/qt/qtdeclarative/commit/1ee7afa50fe5fe995008dee5d00638b894145d12)
- **Author:** Ulf Hermann
- **Optimization:** Cache the attached properties function pointer for faster lookups.
