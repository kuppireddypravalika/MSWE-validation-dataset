#include <vector>
#include <numeric>
#include <immintrin.h> // For SIMD instructions

namespace topi {
namespace generic {
// Optimized matrix multiplication using SIMD
inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    std::fill(C.begin(), C.end(), 0.0); // Initialize C to 0
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a_ik = A[i * n + k];
            
            // Use SIMD to process 4 elements at a time
            for (int j = 0; j < n; j += 4) {
                __m256d b = _mm256_loadu_pd(&B[k * n + j]); // Load 4 doubles
                __m256d c = _mm256_loadu_pd(&C[i * n + j]);
                __m256d a_ik_vec = _mm256_set1_pd(a_ik);
                c = _mm256_add_pd(c, _mm256_mul_pd(a_ik_vec, b));
                _mm256_storeu_pd(&C[i * n + j], c); // Store result
            }
        }
    }
}
} // namespace generic
} // namespace topi

// Wrapper executing the optimized schedule many times
double run_benchmark(int n, int inner_repeat) {
    std::vector<double> A(n * n, 1.0), B(n * n, 2.0), C(n * n);
    for (int r = 0; r < inner_repeat; ++r)
        topi::generic::schedule_quantized_mm_dequantize(A, B, C, n);
    return std::accumulate(C.begin(), C.end(), 0.0);
}

// Explicit template instantiation statements if any
