#include <vector>

// Optimized implementation of flatten_sum
unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    size_t total_size = 0;
    for (const auto& vec : data) {
        total_size += vec.size();
    }
    std::vector<int> flattened(total_size);
    size_t index = 0;
    for (const auto& vec : data) {
        for (int value : vec) {
            flattened[index++] = value;
        }
    }
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < flattened.size(); ++i) {
            sum += flattened[i];
        }
    }
    return sum;
}