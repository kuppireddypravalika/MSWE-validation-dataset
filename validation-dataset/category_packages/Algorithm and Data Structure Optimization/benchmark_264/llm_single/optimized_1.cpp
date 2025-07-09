// optimized.cpp
#include <cmath>
#include <cstddef>
#include <execution>
#include <numeric>
#include <vector>

namespace emp {

// Optimized double power implementation using std::pow
static inline double PowDoubleImpl(double base, double exp) {
    return std::pow(base, exp);
}

// Sum over an array using optimized Pow function
double run(const double* data, std::size_t len, double exponent) {
    // Use a vector to hold data for parallel processing
    std::vector<double> results(len);

    // Use parallel execution policy to speed up the power calculation and summation
    std::transform(std::execution::par, data, data + len, results.begin(), [exponent](double value) {
        return PowDoubleImpl(value, exponent);
    });

    // Use std::reduce for sum with parallel execution policy
    return std::reduce(std::execution::par, results.begin(), results.end(), 0.0);
}

} // namespace emp

// Explicit template instantiation statements
#include <emp/base/vector.hpp>
#include <emp/base/array.hpp>
#include <emp/base/matrix.hpp>
#include <emp/base/array_sequence.hpp>
#include <emp/base/utility.hpp>
#include <emp/base/type_traits.hpp>
#include <emp/base/numeric_limits.hpp>
#include <emp/base/eval.hpp>
#include <emp/tools/string_utils.hpp>
#include <emp/tools/stream.hpp>
#include <emp/tools/dynamic_array.hpp>
#include <emp/tools/robotics.hpp>
#include <emp/tools/json.hpp>
#include <emp/tools/hybrid.hpp>
#include <emp/tools/event.hpp>
