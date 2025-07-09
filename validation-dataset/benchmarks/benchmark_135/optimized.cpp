#include <vector>
#include <cstdint>
#include <limits>

static inline int custom_isnan_optimized(float f) {
    union { float f; uint32_t x; } u = { f };
    return (~u.x << 1) < 0x00fffffeu;
}

unsigned long long count_nan(const std::vector<float>& data, int repeat) {
    unsigned long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (float v : data) {
            sum += custom_isnan_optimized(v);
        }
    }
    return sum;
}
