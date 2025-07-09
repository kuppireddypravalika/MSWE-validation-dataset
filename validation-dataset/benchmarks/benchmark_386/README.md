# Benchmark 386: SPIRVReader address space lookup

This benchmark originates from [commit b2a538c](https://github.com/GPUOpen-Drivers/llpc/commit/b2a538c94c91e6cb9a3ea37c3bb646b7e17a4196) in the LLPC project.
The change modifies `SPIRVReader` so that the address space of a translated
`OpVariable` is taken directly from the pointer type instead of being
recomputed from the storage class. This avoids an extra mapping step for each
variable.
