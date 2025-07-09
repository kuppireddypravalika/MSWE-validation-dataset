# Benchmark 313: vnl_inverse vs vnl_matrix_inverse

This benchmark is derived from commit [85e9585](https://github.com/vxl/vxl/commit/85e9585d24157fc99cadc3757ad4051c103606e3) in the VXL project. The patch replaced the generic `vnl_matrix_inverse` class with the lighter `vnl_inverse` function when inverting 3x3 matrices during image rectification.

The original code constructed a `vnl_matrix_inverse` object for each matrix inversion which internally performs a singular value decomposition. The optimized version uses `vnl_inverse`, a specialized routine for small matrices that avoids the heavy SVD machinery.

This benchmark models that behaviour by repeatedly rectifying two images using either a slow SVD-based inverse or a direct closed-form inverse. The generated output images are identical, but the optimized implementation avoids the expensive SVD computation.

