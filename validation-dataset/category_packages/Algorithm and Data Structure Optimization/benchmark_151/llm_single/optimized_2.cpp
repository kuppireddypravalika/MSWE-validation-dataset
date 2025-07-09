#include <cmath>
#include <vector>
#include <algorithm>

// Lightweight version of the elliptic integral computation
static inline double lightweight_ellint_1(double k)
{
    // Assuming k is valid in the range [0, 1)
    double k2 = k * k;
    return std::comp_ellint_1(k); // Direct call to the library function
}

// Optimized implementation that avoids redundant calls
double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    result.reserve(data.size() * inner);
    std::vector<double> ks(data.size());

    for (int r = 0; r < inner; ++r) {
        std::transform(data.begin(), data.end(), ks.begin(), [r](double x) {
            return std::abs(std::sin(x + r * 0.001));
        });
        for (double k : ks) {
            result += lightweight_ellint_1(k);
        }
    }
    return result;
}
