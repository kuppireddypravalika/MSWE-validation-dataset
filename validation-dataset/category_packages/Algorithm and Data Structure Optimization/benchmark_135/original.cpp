#include <vector>
#include <cstdint>
#include <limits>

static inline int custom_isnan_original(float x) {
    volatile float y = x; // force load to avoid optimizations, mimicking slower macro behaviour
    return !(y == y);
}

unsigned long long count_nan(const std::vector<float>& data, int repeat) {
    unsigned long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (float v : data) {
            sum += custom_isnan_original(v);
        }
    }
    return sum;
}
