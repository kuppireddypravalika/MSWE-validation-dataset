# Benchmark 327: Blacklist fragment after excessive type mismatches

This benchmark reproduces the performance issue fixed in [SpiderMonkey commit `7e95e97`](https://github.com/mozilla/gecko-dev/commit/7e95e972d8ef820338df9d8f1bb6efe9ba3afafb).
The original implementation flushed the JIT fragment on repeated type mismatches but kept trying to
record it again, leading to expensive `js_TrashTree` calls on every iteration. The patch blacklists the
fragment once the mismatch limit is exceeded so subsequent executions skip the costly work.

The benchmark simulates these behaviors with a minimal `Fragment` and `TreeInfo` setup. `execute_tree`
always fails a type check and triggers heavy work when the mismatch counter exceeds a threshold.
In the optimized version the fragment is blacklisted, preventing further expensive calls.

