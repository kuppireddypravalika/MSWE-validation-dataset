#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>

float median(std::vector<float> vals) {
    if (vals.empty()) {
        throw std::invalid_argument("The input vector is empty.");
    }
    size_t n = vals.size();
    std::nth_element(vals.begin(), vals.begin() + n / 2, vals.end());
    if (n % 2 == 0) {
        std::nth_element(vals.begin(), vals.begin() + n / 2 - 1, vals.end());
        return (vals[n / 2] + vals[n / 2 - 1]) / 2.0f;
    } else {
        return vals[n / 2];
    }
}
