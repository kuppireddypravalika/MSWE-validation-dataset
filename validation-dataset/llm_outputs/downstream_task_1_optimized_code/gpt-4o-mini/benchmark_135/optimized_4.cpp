#include <vector>
#include <cstdint>
#include <limits>

static inline bool is_nan(float x) {
    return x != x;
}

unsigned long long count_nan(const std::vector<float>& data, int repeat) {
    unsigned long long sum = 0;
    size_t n = data.size();
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < n; ++i) {
            sum += is_nan(data[i]);
        }
    }
    return sum;
}

// Explicit template instantiation statements
