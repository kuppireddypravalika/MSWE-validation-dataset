#include <cmath>
#include <vector>

// Optimized version uses the C++17 standard library implementation
// std::comp_ellint_1 which is typically faster than the Boost variant.

double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    for (int r = 0; r < inner; ++r) {
        for (double x : data) {
            double k = std::abs(std::sin(x + r * 0.001));
            result += std::comp_ellint_1(k);
        }
    }
    return result;
}
