#include <vector>
#include <algorithm>
#include <stdexcept>

float compute_median(std::vector<float> data) {
    if (data.empty()) {
        throw std::invalid_argument("Input data should not be empty.");
    }
    std::sort(data.begin(), data.end());
    size_t mid = data.size() / 2;
    return (data.size() % 2 == 0) ? (data[mid - 1] + data[mid]) / 2 : data[mid];
}
