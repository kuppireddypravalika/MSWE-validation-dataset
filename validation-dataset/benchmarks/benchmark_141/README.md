# Benchmark 141: Avoid range-loop copies for SetupPayload

This benchmark is based on a change in the CHIP project applying
clang-tidy's `performance-for-range-copy` check.
The original code iterated over `std::vector` elements by value inside
`SetupPayload::operator==`, causing every loop iteration to copy a
`OptionalQRCodeInfo` or `OptionalQRCodeInfoExtension` structure.
Those structures contain `std::string` members, so the copies were
costly.  The patch changed the loops to take the element by `const
&`, removing these unnecessary copies.

Commit: 63e66e1a9a125de60f3c29d9346f04c2caf7d529
