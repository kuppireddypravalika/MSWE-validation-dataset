#include <vector>
#include <numeric>
#include <string>

namespace topi {
namespace generic {
inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += A[i * n + k] * B[k * n + j];
            C[i * n + j] = sum;
        }
}
} // namespace generic

namespace x86 {
// Faster blocked implementation representing a specialized schedule
inline void schedule_quantized_mm_dequantize(const std::vector<double>& A,
                                             const std::vector<double>& B,
                                             std::vector<double>& C,
                                             int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j += 4) {
            double sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
            for (int k = 0; k < n; ++k) {
                double a = A[i * n + k];
                sum0 += a * B[k * n + j];
                sum1 += a * B[k * n + j + 1];
                sum2 += a * B[k * n + j + 2];
                sum3 += a * B[k * n + j + 3];
            }
            C[i * n + j] = sum0;
            C[i * n + j + 1] = sum1;
            C[i * n + j + 2] = sum2;
            C[i * n + j + 3] = sum3;
        }
    }
}
} // namespace x86
} // namespace topi

namespace tvm {
class GenericFunc {
public:
    using FType = void(*)(const std::vector<double>&,
                          const std::vector<double>&,
                          std::vector<double>&,
                          int);
    static GenericFunc& Get(const std::string&) {
        static GenericFunc inst;
        static bool init = false;
        if (!init) {
            inst.func = topi::x86::schedule_quantized_mm_dequantize;
            init = true;
        }
        return inst;
    }
    void operator()(const std::vector<double>& A,
                    const std::vector<double>& B,
                    std::vector<double>& C,
                    int n) const {
        func(A, B, C, n);
    }
private:
    FType func = nullptr;
};
} // namespace tvm

// Optimized version retrieves the specialized schedule via GenericFunc
double run_benchmark(int n, int inner_repeat) {
    std::vector<double> A(n * n, 1.0), B(n * n, 2.0), C(n * n);
    auto& sched = tvm::GenericFunc::Get("schedule_quantized_mm_dequantize");
    for (int r = 0; r < inner_repeat; ++r)
        sched(A, B, C, n);
    return std::accumulate(C.begin(), C.end(), 0.0);
}
