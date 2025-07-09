#include <cmath>
#include <vector>

// Simulate the heavier Boost implementation of the complete elliptic integral
// by performing many redundant computations while ultimately returning the
// standard library result.
static double boost_like_ellint_1(double k)
{
    double result = std::comp_ellint_1(k); // actual value
    const int n = 1000;                    // artificial workload
    volatile double dummy = 0.0;
    for (int i = 0; i < n; ++i) {
        double theta = (i + 0.5) * (M_PI / 2.0) / n;
        dummy += 1.0 / std::sqrt(1.0 - k * k * std::sin(theta) * std::sin(theta));
    }
    (void)dummy; // prevent optimization of the loop
    return result;
}

// Original implementation repeatedly calls the heavy Boost version inside loops.
double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    for (int r = 0; r < inner; ++r) {
        for (double x : data) {
            double k = std::abs(std::sin(x + r * 0.001));
            result += boost_like_ellint_1(k);
        }
    }
    return result;
}

