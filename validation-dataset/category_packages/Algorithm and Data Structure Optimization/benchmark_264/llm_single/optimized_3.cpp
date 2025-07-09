#include <cmath>
#include <cstddef>
#include <type_traits>
#include <immintrin.h>  // For SIMD intrinsics if applicable

namespace emp {

namespace internal {

// Optimized power function
static inline double PowDoubleImpl(double base, double exp) {
    // Efficient exponentiation using logarithm and exponentiation by squaring
    if (base == 0.0) return 0.0;
    if (base < 0.0 && static_cast<int>(exp) != exp) return NAN; // Manage negative bases with non-integer exponents

    if (exp == 0.0) return 1.0;

    double result = 1.0;
    bool negative_exp = false;
    if (exp < 0.0) {
        exp = -exp;
        negative_exp = true;
    }

    // Exponentiation by squaring
    while (exp > 0.0) {
        if (static_cast<int>(exp) % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2.0;
    }

    return negative_exp ? 1.0 / result : result;
}

template <typename T> struct identity { using type = T; };

} // namespace internal

// Optimized version of run using the improved power function
double run(const double* data, std::size_t len, double exponent) {
    double sum = 0.0;
    if (len == 0) return sum; // Early return for empty arrays

    // Vectorize the operation if possible (simple loop unrolling can be done here)
    for (std::size_t i = 0; i < len; ++i) {
        sum += internal::PowDoubleImpl(data[i], exponent);
    }
    return sum;
}

} // namespace emp

// Explicit template instantiation
template double emp::internal::identity<double>::type;
