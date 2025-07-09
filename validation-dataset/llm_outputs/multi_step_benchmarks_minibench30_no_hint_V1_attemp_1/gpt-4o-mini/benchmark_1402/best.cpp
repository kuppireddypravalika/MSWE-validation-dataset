#include "tensor.hpp"
#include <algorithm>
#include <vector>
#include <limits>

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k) {
    std::size_t row = input.rows;
    std::size_t col = input.cols;
    for (std::size_t i = 0; i < row; ++i) {
        // Min-heap for maintaining the top k elements
        std::vector<std::pair<float, std::size_t>> minHeap;
        minHeap.reserve(k);

        // Fill the heap with the first k elements
        for (std::size_t j = 0; j < k; ++j) {
            minHeap.emplace_back(input(i, j), j);
        }
        std::make_heap(minHeap.begin(), minHeap.end(), std::greater<>());

        // Process remaining elements
        for (std::size_t j = k; j < col; ++j) {
            if (input(i, j) > minHeap.front().first) {
                std::pop_heap(minHeap.begin(), minHeap.end(), std::greater<>());
                minHeap.pop_back();
                minHeap.emplace_back(input(i, j), j);
                std::push_heap(minHeap.begin(), minHeap.end(), std::greater<>());
            }
        }
        // Now minHeap contains the k largest elements
        std::sort_heap(minHeap.begin(), minHeap.end(), std::greater<>());

        // Write results to output and indices
        for (std::size_t j = 0; j < k; ++j) {
            output(i, j) = minHeap[j].first;
            indices(i, j) = static_cast<int64_t>(minHeap[j].second);
        }
    }
}

// Explicit template instantiations
template struct Tensor<float>;
template struct Tensor<int64_t>;