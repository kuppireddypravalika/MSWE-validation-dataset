#include <vector>
#include <optional>

// Flatten using optional assignment of temporaries
unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        std::optional<std::vector<int>> inner;
        std::optional<size_t> inner_idx;
        size_t outer_idx = 0;

        auto satisfy = [&](size_t& idx) {
            while (idx < data.size()) {
                // Construct temporary optional then assign
                inner = std::optional<std::vector<int>>(data[idx]);
                inner_idx = std::optional<size_t>(0);
                if (*inner_idx < inner->size()) {
                    return;
                }
                ++idx;
            }
        };

        satisfy(outer_idx);
        while (outer_idx < data.size()) {
            sum += (*inner)[*inner_idx];
            ++*inner_idx;
            if (*inner_idx == inner->size()) {
                ++outer_idx;
                satisfy(outer_idx);
            }
        }
    }
    return sum;
}
