#include <vector>
#include <algorithm>

float compute_median(std::vector<float> data) {
    size_t mid = data.size() / 2;
    std::nth_element(data.begin(), data.begin() + mid, data.end());
    return data[mid];
}
