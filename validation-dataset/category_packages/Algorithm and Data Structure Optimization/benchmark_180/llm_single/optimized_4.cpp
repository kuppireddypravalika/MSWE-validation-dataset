#include <vector>
#include <numeric>
#include <immintrin.h> // For AVX intrinsics, if supported
#include <chrono>

namespace topi {
namespace generic {

inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    std::fill(C.begin(), C.end(), 0.0); // Initialize C to zero 
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a = A[i * n + k];
            for (int j = 0; j < n; ++j) {
                C[i * n + j] += a * B[k * n + j];
            }
        }
    }
}

} // namespace generic
} // namespace topi

// Wrapper executing the slow schedule many times
double run_benchmark(int n, int inner_repeat) {
    std::vector<double> A(n * n, 1.0), B(n * n, 2.0), C(n * n);
    // Use reserve to optimize memory allocations
    C.reserve(n * n);
    for (int r = 0; r < inner_repeat; ++r)
        topi::generic::schedule_quantized_mm_dequantize(A, B, C, n);
    return std::accumulate(C.begin(), C.end(), 0.0);
}

// Explicit template instantiation statements
