#include <cmath>
#include <vector>
#include <numeric>

// Simulate the heavier Boost implementation of the complete elliptic integral
// by performing many redundant computations while ultimately returning the
// standard library result.
static double boost_like_ellint_1(double k)
{
    double result = std::comp_ellint_1(k); // actual value
    const int n = 1000;                    // artificial workload
    double dummy = 0.0;
    for (int i = 0; i < n; ++i) {
        double theta = (i + 0.5) * (M_PI / 2.0) / n;
        dummy += 1.0 / std::sqrt(1.0 - k * k * std::sin(theta) * std::sin(theta));
    }
    return result;
}

// Optimized implementation that reduces redundant computations
double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    const double r_increment = 0.001;
    int data_size = data.size(); // Cache size of data vector
    for (int r = 0; r < inner; ++r) {
        double r_factor = r * r_increment; // Precompute r factor
        for (int i = 0; i < data_size; ++i) {
            double k = std::abs(std::sin(data[i] + r_factor));
            result += boost_like_ellint_1(k);
        }
    }
    return result;
}

// Explicit template instantiation statements
