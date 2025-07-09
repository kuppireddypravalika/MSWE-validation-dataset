#include <cmath>
#include <cstddef>
#include <type_traits>

namespace emp {

namespace internal {

// Optimized integer power implementation
static constexpr double PowInt(double base, std::size_t exp) {
    double result = 1.0;
    while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

// Optimized double power implementation
static double PowDouble(double base, double exp) {
    return std::pow(base, exp);
}

template <typename T> struct identity { using type = T; };

} // namespace internal

// Optimized Pow function
template<typename T>
static constexpr decltype(auto) Pow(
    T base, typename internal::identity<T>::type exp
) {
    if constexpr (std::is_integral<T>::value) {
        return internal::PowInt(base, exp);
    } else {
        return internal::PowDouble(base, exp);
    }
}

// Optimized sum over an array using Pow
double run(const double* data, std::size_t len, double exponent) {
    double sum = 0.0;
    for (std::size_t i = 0; i < len; ++i) {
        sum += Pow(data[i], exponent);
    }
    return sum;
}

} // namespace emp

// Explicit template instantiation statements
template double emp::Pow<double>(double, typename emp::internal::identity<double>::type);
template double emp::Pow<int>(int, typename emp::internal::identity<int>::type);
