# Benchmark 116: Verilator extend operator expansion

This benchmark models a performance fix from the Verilator project where unnecessary cloning
of the left-hand side expression occurred when expanding `EXTEND` assignments.
The original implementation copied the entire assignment LHS for each word,
while the optimized version clones only the expression being extended.

- **Commit:** a554dd7c8d031268f22c4cee0ee7118d6614bd2d
- **Optimization:** Avoid cloning the full LHS when expanding an extend operation.
