#include <cmath>
#include <vector>

// Simulate the heavier Boost implementation of the complete elliptic integral
// by performing many redundant computations while ultimately returning the
// standard library result.
static double boost_like_ellint_1(double k)
{
    double result = std::comp_ellint_1(k); // actual value
    return result;
}

// Optimized implementation that reduces redundant computations.
double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    const size_t data_size = data.size();
    std::vector<double> k_values(inner * data_size);

    // Precompute all k values
    for (int r = 0; r < inner; ++r) {
        for (size_t i = 0; i < data_size; ++i) {
            k_values[r * data_size + i] = std::abs(std::sin(data[i] + r * 0.001));
        }
    }

    // Compute the elliptic integrals in one go
    for (double k : k_values) {
        result += boost_like_ellint_1(k);
    }

    return result;
}

// Unified entry point used by the benchmark harness
double algorithm(const std::vector<double>& data, int inner) {
    return ellint_algorithm(data, inner);
}

// Explicit template instantiation statements.
