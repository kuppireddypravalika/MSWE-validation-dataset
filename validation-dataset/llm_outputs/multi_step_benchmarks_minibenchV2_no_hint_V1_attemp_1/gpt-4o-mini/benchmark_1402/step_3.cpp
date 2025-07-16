#include "tensor.hpp"
#include <algorithm>
#include <vector>
#include <utility>
#include <limits>

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k) {
    std::size_t row = input.rows;
    std::size_t col = input.cols;
    for (std::size_t i = 0; i < row; ++i) {
        std::vector<std::pair<float, std::size_t>> vec;
        vec.reserve(col);
        for (std::size_t j = 0; j < col; ++j) {
            vec.emplace_back(input(i, j), j);
        }
        // Use std::nth_element to find the top-k elements more efficiently
        std::nth_element(vec.begin(), vec.begin() + k, vec.end(), std::greater<>());
        // Sort only the top-k elements for correct order
        std::sort(vec.begin(), vec.begin() + k, std::greater<>());
        for (std::size_t j = 0; j < k; ++j) {
            output(i, j) = vec[j].first;
            indices(i, j) = static_cast<int64_t>(vec[j].second);
        }
    }
}

// Explicit template instantiations
// Assuming previous explicit template instantiations are required. Reproduce here if they exist in the original code.