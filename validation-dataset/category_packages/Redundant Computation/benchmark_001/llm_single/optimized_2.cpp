// original.cpp
#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

template <typename T>
void logsumexp(const Eigen::Tensor<T, 2>& x, Eigen::Tensor<T, 1>& y, const int dim) {
    Eigen::array<int, 1> reduction_dims = {dim};

    // Compute the maximum values along the specified dimension
    auto x_max = x.maximum(reduction_dims);
    // Calculate the difference from the maximum, exponentiate, and sum
    auto x_max_broadcasted = x_max.reshape(Eigen::array<Eigen::Index, 2>{x.dimension(0), 1}).broadcast(Eigen::array<Eigen::Index, 2>{1, x.dimension(1)});
    auto exp_diff = (x - x_max_broadcasted).exp().sum(reduction_dims);
    // Calculate log-sum-exp efficiently
    y = (x_max + exp_diff.log()).reshape(y.dimensions());
}

// Explicit instantiation
template void logsumexp<float>(const Eigen::Tensor<float, 2>&, Eigen::Tensor<float, 1>&, const int);
