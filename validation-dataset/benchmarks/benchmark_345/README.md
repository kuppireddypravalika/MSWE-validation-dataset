# Benchmark 345: Memory profiling macro optimization

This benchmark is based on [commit 13038329](https://github.com/doublec/nanojit-central/commit/1303832925680c90ecccff949ae9aa82d5113e73) from Adobe's Nanojit project. The patch replaced expensive `MMGC_MEM_TYPE` calls with lightweight `MMGC_MEM_TAG` versions inside `Fragmento::pagesGrow`.

Our simplified code imitates this by implementing two macros that perform different amounts of work. The original version repeatedly concatenates strings in `MMGC_MEM_TYPE` whereas the optimized version simply counts characters in `MMGC_MEM_TAG`. Both versions allocate a number of pages each iteration. The harness measures the time to invoke `pagesGrow` many times.
