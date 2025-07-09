#include "matrix.hpp"

// Optimized computation using specialized symmetric product

double jacobian_product(const Matrix& J) {
    Matrix M(J.rows, J.rows);
    for (std::size_t i = 0; i < J.rows; ++i) {
        for (std::size_t j = i; j < J.rows; ++j) {
            double sum = 0.0;
            for (std::size_t k = 0; k < J.cols; ++k) {
                sum += J(i, k) * J(j, k);
            }
            M(i, j) = sum;
            if (i != j) M(j, i) = sum;
        }
    }

    double total = 0.0;
    for (std::size_t i = 0; i < M.rows; ++i)
        for (std::size_t j = 0; j < M.cols; ++j)
            total += M(i, j);
    return total;
}

