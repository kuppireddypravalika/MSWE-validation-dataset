# Benchmark 145: Integer overflow in domain decomposition

This benchmark is based on Gromacs commit [2b1918e7](https://gitlab.com/gromacs/gromacs/-/commit/2b1918e7483649fcc3a47cc55ffd7dc54ad7a9fb) which fixes an integer overflow in the domain decomposition setup. The original implementation used 32‑bit integers when checking for large prime factors of the process grid:

```c
if (ldiv*ldiv*ldiv > nnodes_div*nnodes_div)
    ...
```

With more than ~46k MPI ranks this multiplication overflowed, causing the condition to fail and leading to extremely slow grid search. The patch changes `ldiv` and `nnodes_div` to a 64‑bit type so the comparison works correctly.

This benchmark replicates that logic and measures the runtime difference when the overflow occurs.
