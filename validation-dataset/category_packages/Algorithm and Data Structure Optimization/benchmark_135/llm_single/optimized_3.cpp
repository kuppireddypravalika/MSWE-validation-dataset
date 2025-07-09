#include <vector>
#include <cstdint>
#include <limits>

static inline int custom_isnan_optimized(float x) {
    return x != x;
}

unsigned long long count_nan(const std::vector<float>& data, int repeat) {
    unsigned long long sum = 0;
    size_t dataSize = data.size();
    if (dataSize == 0) return 0;

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < dataSize; ++i) {
            sum += custom_isnan_optimized(data[i]);
        }
    }
    return sum;
}
