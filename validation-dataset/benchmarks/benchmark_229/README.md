# Benchmark 229: vm_acquire translation cache allocation

This benchmark is based on commit [599f7e8](https://github.com/cebix/BasiliskII/commit/599f7e845f9da8ae11fd5938f4eb11683c02cf38) from the BasiliskII project.
The patch replaced direct usage of `mmap`/`malloc` with a cross-platform
`vm_acquire()` helper when allocating the JIT translation cache and applied
`vm_protect()` afterwards.

The goal is to measure the impact of using the specialized allocation helper
versus the original manual `mmap` approach.
