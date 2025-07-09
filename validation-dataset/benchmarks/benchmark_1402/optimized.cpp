#include "tensor.hpp"
#include <algorithm>
#include <vector>
#ifdef PADDLE_WITH_MKLDNN
#include <omp.h>
#endif

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k) {
    std::size_t row = input.rows;
    std::size_t col = input.cols;
#ifdef PADDLE_WITH_MKLDNN
#pragma omp parallel for
#endif
    for (std::size_t i = 0; i < row; ++i) {
        std::vector<std::pair<float, std::size_t>> vec;
        vec.reserve(col);
        for (std::size_t j = 0; j < col; ++j) {
            vec.emplace_back(input(i, j), j);
        }
        std::partial_sort(vec.begin(), vec.begin() + k, vec.end(),
                          [](const auto& l, const auto& r) { return l.first > r.first; });
        for (std::size_t j = 0; j < k; ++j) {
            output(i, j) = vec[j].first;
            indices(i, j) = static_cast<int64_t>(vec[j].second);
        }
    }
}
