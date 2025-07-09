# Benchmark 310: Set Page Table A/D bits

This benchmark is derived from commit [fae787c](https://github.com/chelsix/xv6/commit/fae787c58a02ecf3eae6711043710fc6d87e1aa7) of the xv6-riscv kernel. The patch ensures that newly created page table entries have their Accessed (A) and Dirty (D) bits set when the entry is marked present. Without these bits the CPU must update them using microcode assists on the first access which is significantly slower.

The simplified model here repeatedly inserts page table entries and then touches the mapped pages. The original version omits the A/D bits causing a penalty on every first access while the optimized version sets them eagerly.
