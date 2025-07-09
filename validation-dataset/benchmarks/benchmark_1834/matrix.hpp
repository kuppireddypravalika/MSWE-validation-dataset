#pragma once
#include <vector>
#include <cstddef>

struct Matrix {
    std::size_t rows, cols;
    std::vector<double> data;
    Matrix(std::size_t r=0, std::size_t c=0)
        : rows(r), cols(c), data(r*c) {}
    double& operator()(std::size_t r, std::size_t c) {
        return data[r*cols + c];
    }
    const double& operator()(std::size_t r, std::size_t c) const {
        return data[r*cols + c];
    }
};
