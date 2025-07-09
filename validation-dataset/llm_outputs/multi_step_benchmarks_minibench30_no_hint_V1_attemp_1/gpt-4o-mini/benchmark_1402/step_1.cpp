#include "tensor.hpp"
#include <algorithm>
#include <vector>
#include <limits>

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k) {
    std::size_t row = input.rows;
    std::size_t col = input.cols;
    for (std::size_t i = 0; i < row; ++i) {
        // Use a min-heap to store the top k elements
        std::vector<std::pair<float, std::size_t>> vec;
        vec.reserve(k);
        for (std::size_t j = 0; j < k; ++j) {
            vec.emplace_back(input(i, j), j);
        }
        // Process remaining elements
        for (std::size_t j = k; j < col; ++j) {
            if (input(i, j) > vec.front().first) {
                std::pop_heap(vec.begin(), vec.end(), std::greater<>());
                vec.pop_back();
                vec.emplace_back(input(i, j), j);
                std::push_heap(vec.begin(), vec.end(), std::greater<>());
            }
        }
        // Sort the top k results
        std::sort(vec.begin(), vec.end(), std::greater<>());
        for (std::size_t j = 0; j < k; ++j) {
            output(i, j) = vec[j].first;
            indices(i, j) = static_cast<int64_t>(vec[j].second);
        }
    }
}


// Explicit template instantiations
#include <string>
template struct Tensor<float>;
template struct Tensor<int64_t>;