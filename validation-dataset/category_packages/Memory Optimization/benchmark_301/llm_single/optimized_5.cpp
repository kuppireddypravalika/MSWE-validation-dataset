#include <vector>
#include <numeric>

unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat) {
    unsigned long long sum = 0;
    size_t total_size = 0;

    // Pre-calculate total size for all vectors in data
    for (const auto& vec : data) {
        total_size += vec.size();
    }

    // Create a flat array to facilitate faster access
    std::vector<int> flat_data;
    flat_data.reserve(total_size);
    for (const auto& vec : data) {
        flat_data.insert(flat_data.end(), vec.begin(), vec.end());
    }

    // Calculate sum for the repeated flattening
    size_t flat_size = flat_data.size();
    for (int r = 0; r < repeat; ++r) {
        sum += std::accumulate(flat_data.begin(), flat_data.end(), 0ULL);
    }

    return sum;
}
