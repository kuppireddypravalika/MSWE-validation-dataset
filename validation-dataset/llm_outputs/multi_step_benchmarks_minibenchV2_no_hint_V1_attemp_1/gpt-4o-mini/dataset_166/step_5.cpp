#include <algorithm>
#include <vector>
#include <utility>
#include <limits>

void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices) {
    output.resize(row * k);
    indices.resize(row * k);
    
    for (size_t i = 0; i < row; ++i) {
        std::vector<std::pair<float, size_t>> vec;
        vec.reserve(k); // Reserve space for k pairs
        size_t count = 0;
        
        for (size_t j = 0; j < col; ++j) {
            float value = input[i * col + j];
            if (count < k) {
                vec.emplace_back(value, j);
                count++;
            } else if (value > vec.back().first) {
                vec.back() = std::make_pair(value, j);
            }
            if (count == k) {
                std::partial_sort(vec.begin(), vec.begin() + k, vec.end(), 
                                  [](const auto& l, const auto& r) {
                                      return l.first > r.first;
                                  });
            }
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