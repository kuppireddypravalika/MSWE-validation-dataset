# Benchmark_012: Static Tokenizer Reuse in AegisubDC

This benchmark captures a real-world performance optimization made in the AegisubDC subtitle editor. The commit avoids unnecessary reconstruction of the tokenizer used to parse dialogue lines by switching from on-demand allocation to static reuse of pre-configured instances.

- **Commit:** [92b8b285](https://github.com/Ristellise/AegisubDC/commit/92b8b2851b0e12c82269d59e1bfee1beaaddca54)
- **Author:** Thomas Goyne
- **Optimization Type:** Execution-Time Inefficiency
- **Optimization Summary:** Replaces `dialogue_tokens(...)` temporary object with a static instance (`kt` or `not_kt`) selected based on a boolean flag. This avoids repeated construction overhead in `TokenizeDialogueBody()`.

## Benchmark Goal

Demonstrate the runtime savings from reusing expensive tokenizer objects across repeated invocations. In the original implementation, a new tokenizer is constructed for every call. The optimized version statically initializes two tokenizer variants and selects between them, saving setup time on each call.

This benchmark uses a `MockTokenizer` that simulates real-world lexer setup cost to model the impact of this optimization.

