# Benchmark 370: Direct Eigenvalue Computation

This benchmark originates from the Kratos Multiphysics project. The patch replaces a conditional call to a generic eigenvalue solver with a dedicated direct method for computing eigenvalues of the Cauchy stress tensor.

- **Commit:** [b430a445](https://github.com/KratosMultiphysics/Kratos/commit/b430a4457ade2140f5ae0d1ccb74f01d3435c673)
- **Author:** lgracia
- **Optimization:** Use `EigenValuesDirectMethod` instead of checking the diagonal sum and falling back to a slower routine.

## Benchmark Goal

Measure the performance benefit of using a direct eigenvalue computation over an iterative approach when processing many stress tensors.

