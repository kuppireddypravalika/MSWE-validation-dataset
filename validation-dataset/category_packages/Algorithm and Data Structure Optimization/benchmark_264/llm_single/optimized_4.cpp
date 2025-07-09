// optimized.cpp
#include <cmath>
#include <cstddef>
#include <execution>
#include <numeric>
#include <vector>

namespace emp {

// Efficient double power implementation using std::pow
static double PowDoubleImpl(double base, double exp) {
    return std::pow(base, exp);
}

// Sum over an array using std::transform and std::reduce for parallel computing
double run(const double* data, std::size_t len, double exponent) {
    // Using std::vector to allow dynamic resizing and efficient computations.
    std::vector<double> results(len);
    std::transform(std::execution::par, data, data + len, results.begin(), [exponent](double value) {
        return PowDoubleImpl(value, exponent);
    });
    // Using std::reduce for efficient summation of results.
    return std::reduce(std::execution::par, results.begin(), results.end(), 0.0);
}

} // namespace emp

template void emp::internal::PowIntImpl<int>(int, int);
template void emp::internal::PowIntImpl<long>(long, long);
template void emp::internal::PowIntImpl<long long>(long long, long long);
template double emp::internal::PowDoubleImpl(double, double);
template <typename T> struct emp::internal::identity;
