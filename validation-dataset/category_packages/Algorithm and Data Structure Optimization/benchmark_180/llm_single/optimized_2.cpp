#include <vector>
#include <numeric>
#include <execution>

namespace topi {
namespace generic {
// Optimized matrix multiplication using parallel execution
inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    std::fill(C.begin(), C.end(), 0.0); // Initialize C to zero
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += A[i * n + k] * B[k * n + j];
            C[i * n + j] = sum;
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

// Explicit template instantiation statements
