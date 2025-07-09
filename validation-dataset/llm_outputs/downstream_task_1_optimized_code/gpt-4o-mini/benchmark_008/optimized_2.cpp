#include <vector>
#include <algorithm>
#include <numeric>

float compute_median(std::vector<float> data) {
    if (data.empty()) throw std::invalid_argument("Data must not be empty");
    size_t n = data.size();
    std::nth_element(data.begin(), data.begin() + n / 2, data.end());
    if (n % 2 == 0) {
        float median1 = data[n / 2];
        float median2 = *std::max_element(data.begin(), data.begin() + n / 2);
        return (median1 + median2) / 2.0f;
    } else {
        return data[n / 2];
    }
}

