#include <vector>
#include <algorithm>
#include <stdexcept>

float compute_median(std::vector<float> data) {
    if (data.empty()) {
        throw std::invalid_argument("Input data cannot be empty.");
    }
    std::size_t n = data.size();
    std::nth_element(data.begin(), data.begin() + n / 2, data.end());
    float median = data[n / 2];
    if (n % 2 == 0) {
        std::nth_element(data.begin(), data.begin() + (n / 2) - 1, data.end());
        median = (median + data[(n / 2) - 1]) / 2.0f;
    }
    return median;
}
