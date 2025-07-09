// optimized.cpp
#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>


template <typename T>
void logsumexp(const Eigen::Tensor<T, 2>& x, Eigen::Tensor<T, 1>& y, const int dim) {
    Eigen::array<int, 1> reduction_dims = {dim};

    // Cache dimensions for reuse
    const int rows = x.dimension(0);
    const int cols = x.dimension(1);

    // Compute maximum values along the specified dimension once
    auto x_max = x.maximum(reduction_dims);

    // Expand the maximum tensor only once
    Eigen::Tensor<T, 2> x_max_expanded = x_max.reshape(Eigen::array<Eigen::Index, 2>{rows, 1}).broadcast(Eigen::array<Eigen::Index, 2>{1, cols});

    // Perform the logsumexp computation efficiently
    y = (x_max + ((x - x_max_expanded).exp().sum(reduction_dims).log())).reshape(y.dimensions());
}

// Explicit instantiation
template void logsumexp<float>(const Eigen::Tensor<float, 2>&, Eigen::Tensor<float, 1>&, const int);