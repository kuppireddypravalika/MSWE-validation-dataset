#include <vector>
#include <numeric>
#include <immintrin.h>

namespace topi {
namespace generic {
// Optimized matrix multiplication using SIMD
inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    int i, j, k;
    // Initialize C to zero
    std::fill(C.begin(), C.end(), 0.0);
    const int simd_width = 4; // AVX can process 4 double values at once
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            __m256d sum = _mm256_setzero_pd();
            for (k = 0; k < n; k += simd_width) {
                __m256d vecA = _mm256_loadu_pd(&A[i * n + k]);
                __m256d vecB = _mm256_loadu_pd(&B[k * n + j]);
                sum = _mm256_fmadd_pd(vecA, vecB, sum);
            }
            // Horizontal sum the SIMD result
            double temp[4];
            _mm256_storeu_pd(temp, sum);
            C[i * n + j] = temp[0] + temp[1] + temp[2] + temp[3];
        }
    }
}
} // namespace generic
} // namespace topi

// Wrapper executing the slow schedule many times
double run_benchmark(int n, int inner_repeat) {
    std::vector<double> A(n * n, 1.0), B(n * n, 2.0), C(n * n);
    for (int r = 0; r < inner_repeat; ++r)
        topi::generic::schedule_quantized_mm_dequantize(A, B, C, n);
    return std::accumulate(C.begin(), C.end(), 0.0);
}

// Explicit template instantiation
template void topi::generic::schedule_quantized_mm_dequantize(const std::vector<double>&, const std::vector<double>&, std::vector<double>&, int);
