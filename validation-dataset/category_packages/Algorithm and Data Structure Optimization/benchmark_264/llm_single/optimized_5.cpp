#include <cmath>
#include <cstddef>
#include <immintrin.h> // For SIMD instructions

namespace emp {

namespace internal {

// Efficient power implementation using std::pow directly
inline double PowDoubleImpl(double base, double exp) {
    return std::pow(base, exp); // Use the standard library pow
}

} // namespace internal

// Sum over an array using Pow with SIMD optimizations
double run(const double* data, std::size_t len, double exponent) {
    double sum = 0.0;
    const std::size_t simd_size = 4; // Process 4 doubles at once
    std::size_t i = 0;

    // Process in chunks of 4 using SIMD
    for (; i <= len - simd_size; i += simd_size) {
        __m256d vec_data = _mm256_loadu_pd(&data[i]);
        __m256d vec_exponent = _mm256_set1_pd(exponent);
        __m256d vec_result = _mm256_pow_pd(vec_data, vec_exponent);
        sum += vec_result[0] + vec_result[1] + vec_result[2] + vec_result[3];
    }

    // Process remaining elements
    for (; i < len; ++i) {
        sum += internal::PowDoubleImpl(data[i], exponent);
    }

    return sum;
}

} // namespace emp

// Explicit template instantiation statements
template double emp::internal::Pow<double>(double, double);
