# Dataset 006: avoid redundant rom list sorting

This benchmark is based on a DaedalusX64 commit `7ebdb792` which introduced a check in `DrawRomSelector` to only sort the ROM list when the sort order actually changes. The original code resorted the linked list every frame even if no sorting was required.

The benchmark constructs a singly linked list of ROM entries and repeatedly calls a draw function. The original version runs the bubble sort on every call while the optimized version remembers the previous sort order and only sorts when it differs from the current one.
