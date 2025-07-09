#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

float median(std::vector<float> vals) {
    if (vals.empty()) return NAN;  // Handle edge case for empty input
    size_t n = vals.size();
    std::nth_element(vals.begin(), vals.begin() + n / 2, vals.end());
    if (n % 2 == 0) {
        float median1 = vals[n / 2];
        float median2 = *std::max_element(vals.begin(), vals.begin() + n / 2);
        return (median1 + median2) / 2.0f;
    } else {
        return vals[n / 2];
    }
}

// Explicit template instantiation statements (if any) go here.