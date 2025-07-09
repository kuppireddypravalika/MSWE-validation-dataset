#include <cmath>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <numeric>

namespace emp {

namespace internal {

// More efficient double power implementation using std::pow
static double PowDoubleImpl(double base, double exp) {
    return std::pow(base, exp);
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
    // Use std::transform to optimize summation and reduce looping overhead
    std::vector<double> results(len);
    std::transform(data, data + len, results.begin(), [exponent](double value) {
        return Pow<double>(value, exponent);
    });
    return std::accumulate(results.begin(), results.end(), 0.0);
}

} // namespace emp

// Explicit template instantiation statements (preserved unchanged)
#include <emp/tools/string_utils.hpp>
template double emp::internal::Pow<double>(double, double);
