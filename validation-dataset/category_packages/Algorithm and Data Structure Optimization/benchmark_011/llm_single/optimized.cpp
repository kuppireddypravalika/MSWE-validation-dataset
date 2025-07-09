#include <vector>
#include <algorithm>
#include <stdexcept>

float median(std::vector<float> vals) {
    if (vals.empty()) {
        throw std::invalid_argument("Input vector is empty");
    }
    size_t mid = vals.size() / 2;
    std::nth_element(vals.begin(), vals.begin() + mid, vals.end());
    if (vals.size() % 2 == 0) {
        float mid2;
        std::nth_element(vals.begin(), vals.begin() + mid - 1, vals.end());
        mid2 = vals[mid - 1];
        return (vals[mid] + mid2) / 2.0f;
    }
    return vals[mid];
}

// Explicit template instantiation statements
