# Benchmark 110: Reloading CPU segments with a long jump

This benchmark is based on commit [1bc05db0](https://github.com/SerenityOS/serenity/commit/1bc05db0f260c8479868a1ace37cf29c989e6613) which changed how the BIOS boot code reloads the code segment. The original implementation used a `push` + `retf` sequence followed by another block to set the data segments. The patched version replaced this with a single `ljmp` instruction that reloads the code segment and immediately sets the remaining segments.

Our simplified benchmark models this difference by executing two separate memory loops (representing the two assembly blocks) versus a single combined loop. The workload size is scaled so the original version runs for several seconds, making the improvement measurable.
