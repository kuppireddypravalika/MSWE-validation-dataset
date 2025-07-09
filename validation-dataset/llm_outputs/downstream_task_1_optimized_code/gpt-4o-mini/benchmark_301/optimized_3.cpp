#include <vector>

// Optimized flatten_sum without optional overhead
unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (const auto& inner : data) {
            sum += std::accumulate(inner.begin(), inner.end(), 0ULL);
        }
    }
    return sum;
}