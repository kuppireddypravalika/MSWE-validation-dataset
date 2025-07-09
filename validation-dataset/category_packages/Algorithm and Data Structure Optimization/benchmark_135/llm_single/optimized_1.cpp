#include <vector>
#include <cstdint>
#include <limits>

static inline int custom_isnan_optimized(float x) {
    return x != x;
}

unsigned long long count_nan(const std::vector<float>& data, int repeat) {
    unsigned long long sum = 0;
    size_t data_size = data.size();
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < data_size; ++i) {
            sum += custom_isnan_optimized(data[i]);
        }
    }
    return sum;
}

// Explicit template instantiation statements unchanged:
// template class std::vector<float>;
