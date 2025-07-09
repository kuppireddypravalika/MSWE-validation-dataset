#include <vector>
#include <algorithm>
#include <iterator>
#include <stdexcept>

float compute_median(std::vector<float> data) {
    if (data.empty()) {
        throw std::invalid_argument("Input vector must not be empty.");
    }
    std::size_t size = data.size();
    std::size_t mid = size / 2;
    std::nth_element(data.begin(), data.begin() + mid, data.end());
    if (size % 2 == 0) {
        float median1 = data[mid];
        float median2;
        std::nth_element(data.begin(), data.begin() + mid - 1, data.end());
        median2 = data[mid - 1];
        return (median1 + median2) / 2.0f;
    } else {
        return data[mid];
    }
}
