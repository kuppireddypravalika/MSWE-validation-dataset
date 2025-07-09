#include <vector>
#include <algorithm>

float compute_median(std::vector<float> data) {
    std::sort(data.begin(), data.end());
    size_t mid = data.size() / 2;
    return data[mid];
}
