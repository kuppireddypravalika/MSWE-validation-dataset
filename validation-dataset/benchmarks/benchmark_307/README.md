# Benchmark 307: Block assignment optimization

This benchmark is derived from commit [14ed79a](https://github.com/.../commit/14ed79a191ced90043c5a153de3dc759f8129bdd) which changed how assignments are compiled in a block. The original version recreated entries for assigned variables and manually stored loaded values. The patch introduced `create_entry`, `store_value`, and `get_value` helper methods on the `Variable` class, reducing redundant allocations and loads.

The goal of this benchmark is to evaluate whether using these helper methods improves performance when many assignments are processed.
