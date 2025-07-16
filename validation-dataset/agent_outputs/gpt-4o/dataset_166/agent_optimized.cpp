#include <algorithm>
#include <vector>
#include <utility>

void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices) {
    output.resize(row * k);
    indices.resize(row * k);
    for (size_t i = 0; i < row; ++i) {
        std::vector<std::pair<float, size_t>> vec;
        vec.reserve(col); // preallocate memory
        for (size_t j = 0; j < col; ++j) {
            vec.push_back(std::pair<float, size_t>(input[i * col + j], j));
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
