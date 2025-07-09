#include <vector>

// Optimized flatten sum function using references instead of optionals
unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        size_t outer_idx = 0;
        size_t inner_idx = 0;

        while (outer_idx < data.size()) {
            if (inner_idx < data[outer_idx].size()) {
                sum += data[outer_idx][inner_idx];
                ++inner_idx;
            } else {
                ++outer_idx;
                inner_idx = 0;
            }
        }
    }
    return sum;
}