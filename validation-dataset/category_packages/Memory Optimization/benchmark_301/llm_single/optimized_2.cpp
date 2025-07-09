#include <vector>

// Optimized flatten_sum function
unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    size_t outer_size = data.size();
    for (int r = 0; r < repeat; ++r) {
        for (size_t outer_idx = 0; outer_idx < outer_size; ++outer_idx) {
            const std::vector<int>& inner = data[outer_idx];
            size_t inner_size = inner.size();
            for (size_t inner_idx = 0; inner_idx < inner_size; ++inner_idx) {
                sum += inner[inner_idx];
            }
        }
    }
    return sum;
}
