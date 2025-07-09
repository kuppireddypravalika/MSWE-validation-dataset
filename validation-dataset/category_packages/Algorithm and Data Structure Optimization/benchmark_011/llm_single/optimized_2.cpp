#include <vector>
#include <algorithm>
#include <iterator>
#include <stdexcept>

float median(std::vector<float> vals) {
    if (vals.empty()) {
        throw std::invalid_argument("Input vector must not be empty");
    }
    size_t n = vals.size();
    std::nth_element(vals.begin(), vals.begin() + n / 2, vals.end());
    if (n % 2 == 0) {
        float mid1 = *std::max_element(vals.begin(), vals.begin() + n / 2);
        return (mid1 + vals[n / 2]) / 2;
    } else {
        return vals[n / 2];
    }
}
