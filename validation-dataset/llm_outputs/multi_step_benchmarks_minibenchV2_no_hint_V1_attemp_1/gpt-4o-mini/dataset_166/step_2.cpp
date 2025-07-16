#include <algorithm>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <memory>

void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices) {
    output.resize(row * k);
    indices.resize(row * k);
    std::vector<std::pair<float, size_t>> vec(k);
    for (size_t i = 0; i < row; ++i) {
        size_t count = 0;
        for (size_t j = 0; j < col; ++j) {
            if (count < k) {
                vec[count++] = std::make_pair(input[i * col + j], j);
            } else if (input[i * col + j] > vec[k - 1].first) {
                vec[k - 1] = std::make_pair(input[i * col + j], j);
            }
            std::partial_sort(vec.begin(), vec.begin() + count, vec.begin() + std::min(count, k), 
                              [](const std::pair<float, size_t>& l,
                                 const std::pair<float, size_t>& r) {
                                     return l.first > r.first;
                              });
        }
        for (size_t j = 0; j < k; ++j) {
            output[i * k + j] = vec[j].first;
            indices[i * k + j] = vec[j].second;
        }
    }
}

// Explicit template instantiations
#include <vector>
template void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices);