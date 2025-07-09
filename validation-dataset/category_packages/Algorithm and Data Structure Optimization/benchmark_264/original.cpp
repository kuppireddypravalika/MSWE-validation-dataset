#include <cmath>
#include <cstddef>
#include <type_traits>

namespace emp {

namespace internal {

// Simple integer power implementation
template<typename T>
static constexpr T PowIntImpl(T base, T exp) {
    return exp <= 0 ? static_cast<T>(1) : base * PowIntImpl(base, exp - 1);
}

// Intentionally inefficient double power implementation
static double PowDoubleImpl(double base, double exp) {
    // Break the computation into many smaller pow calls
    const int loops = 1000;
    double part = std::exp(std::log(base) * (exp / loops));
    double result = 1.0;
    for (int i = 0; i < loops; ++i) result *= part;
    return result;
}

template <typename T> struct identity { using type = T; };

} // namespace internal

// Original version without builtin_constant_p
template<typename T>
static constexpr decltype(auto) Pow(
    T base, typename internal::identity<T>::type exp
) {
    if constexpr (std::is_integral<T>::value) {
        return internal::PowIntImpl(base, exp);
    } else {
        return internal::PowDoubleImpl(base, exp);
    }
}

// Sum over an array using Pow
double run(const double* data, std::size_t len, double exponent) {
    double sum = 0.0;
    for (std::size_t i = 0; i < len; ++i) {
        sum += Pow<double>(data[i], exponent);
    }
    return sum;
}

} // namespace emp
