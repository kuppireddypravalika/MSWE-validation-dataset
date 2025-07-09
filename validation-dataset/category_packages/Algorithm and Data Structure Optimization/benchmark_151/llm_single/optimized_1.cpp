#include <cmath>
#include <vector>

// Improved implementation of the complete elliptic integral
static double boost_like_ellint_1(double k)
{
    return std::comp_ellint_1(k); // return the standard library result directly
}

double ellint_algorithm(const std::vector<double>& data, int inner)
{
    double result = 0.0;
    const int data_size = data.size(); // Cache the size for efficiency
    for (int r = 0; r < inner; ++r) {
        double r_increment = r * 0.001; // Calculate increment outside the inner loop
        for (int i = 0; i < data_size; ++i) {
            double x = data[i]; // Access data using index to improve cache locality
            double k = std::abs(std::sin(x + r_increment));
            result += boost_like_ellint_1(k);
        }
    }
    return result;
}

// Explicit template instantiation statements
