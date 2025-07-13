#include "tensor.hpp"
#include <algorithm>
#include <vector>
#include <queue>

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k) {
    std::size_t row = input.rows;
    std::size_t col = input.cols;
    for (std::size_t i = 0; i < row; ++i) {
        std::priority_queue<std::pair<float, std::size_t>, std::vector<std::pair<float, std::size_t>>, std::greater<std::pair<float, std::size_t>>> min_heap;
        for (std::size_t j = 0; j < col; ++j) {
            if (min_heap.size() < k) {
                min_heap.emplace(input(i, j), j);
            } else if (input(i, j) > min_heap.top().first) {
                min_heap.pop();
                min_heap.emplace(input(i, j), j);
            }
        }
        std::vector<std::pair<float, std::size_t>> vec;
        vec.reserve(k);
        while (!min_heap.empty()) {
            vec.push_back(min_heap.top());
            min_heap.pop();
        }
        std::sort(vec.begin(), vec.end(), [](const auto& l, const auto& r) { return l.first > r.first; });
        for (std::size_t j = 0; j < k; ++j) {
            output(i, j) = vec[j].first;
            indices(i, j) = static_cast<int64_t>(vec[j].second);
        }
    }
}
