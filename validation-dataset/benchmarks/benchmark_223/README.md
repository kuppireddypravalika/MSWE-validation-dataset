# Benchmark 223: Replace strlen with string::size

This benchmark is based on commit [7597c89](https://github.com/ruffina-koza/repo/commit/7597c89a155b30614bd980d38ad63ef22f0820a8) which replaced `strlen(info.name.c_str())` with `info.name.size()` when computing the maximum skill name length.
The change avoids repeatedly scanning the string contents and improves display performance.
