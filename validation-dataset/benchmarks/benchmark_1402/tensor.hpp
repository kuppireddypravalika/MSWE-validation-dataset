#pragma once
#include <vector>

template <typename T>
struct Tensor {
    std::size_t rows{0}, cols{0};
    std::vector<T> data;
    Tensor(std::size_t r=0, std::size_t c=0): rows(r), cols(c), data(r*c) {}
    T& operator()(std::size_t i, std::size_t j) { return data[i*cols + j]; }
    const T& operator()(std::size_t i, std::size_t j) const { return data[i*cols + j]; }
};
