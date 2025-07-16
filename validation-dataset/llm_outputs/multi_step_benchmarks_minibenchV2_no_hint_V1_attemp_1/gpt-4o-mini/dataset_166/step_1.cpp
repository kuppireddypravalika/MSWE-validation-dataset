#include <algorithm>
#include <vector>
#include <utility>
#include <memory> // for std::unique_ptr

void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices) {
    output.resize(row * k);
    indices.resize(row * k);
    std::vector<std::pair<float, size_t>> vec;
    vec.reserve(k); // Reserve space for k elements only
    for (size_t i = 0; i < row; ++i) {
        vec.clear(); // Clear the vector for reuse
        for (size_t j = 0; j < col; ++j) {
            vec.emplace_back(input[i * col + j], j); // use emplace_back for efficiency
        }
        std::partial_sort(vec.begin(), vec.begin() + k, vec.end(),
                          [](const std::pair<float, size_t>& l,
                             const std::pair<float, size_t>& r) {
                              return l.first > r.first;
                          });
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