# Benchmark 164: log auto-fit rate limiting

This benchmark is based on [Code::Blocks commit 4c5ad730](https://github.com/codeblocks-org/codeblocks/commit/4c5ad730172d8422eabba194a0969a91231e4d62). The patch limits how often the compiler log automatically adjusts the width of the "Message" column.

The original implementation auto-fit the column after every appended message, which becomes very slow when thousands of build messages are produced. The optimized version performs the expensive auto-fit at most once every three seconds.

The benchmark mimics this behaviour with a simplified logging class where auto-fitting performs a costly scan over all stored rows. The optimized version avoids the scan for most messages, mirroring the real patch.
