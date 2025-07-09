#include <algorithm>
#include <vector>
#include <utility>

// Optimized top-k implementation with preallocation and efficient data handling

void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices) {
    output.resize(row * k);
    indices.resize(row * k);

    // Use a vector of pairs with pre-reserved space to hold the top k elements
    std::vector<std::pair<float, size_t>> vec;

    for (size_t i = 0; i < row; ++i) {
        vec.clear();
        vec.reserve(k);
        size_t current_size = 0;

        // Collect only the top k elements using a min-heap
        for (size_t j = 0; j < col; ++j) {
            float value = input[i * col + j];
            if (current_size < k) {
                vec.emplace_back(value, j);
                std::push_heap(vec.begin(), vec.begin() + ++current_size, std::greater<>());
            } else if (value > vec.front().first) {
                std::pop_heap(vec.begin(), vec.begin() + current_size, std::greater<>());
                vec[current_size - 1] = {value, j};
                std::push_heap(vec.begin(), vec.begin() + current_size, std::greater<>());
            }
        }

        // Sort the top-k elements in descending order
        std::sort(vec.begin(), vec.begin() + current_size, std::greater<>());

        // Transfer the results to the output arrays
        for (size_t j = 0; j < k; ++j) {
            output[i * k + j] = vec[j].first;
            indices[i * k + j] = vec[j].second;
        }
    }
}

// Explicit template instantiations remain unchanged.