#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

template <typename T>
void logsumexp(const Eigen::Tensor<T, 2>& x, Eigen::Tensor<T, 1>& y, const int dim) {
    Eigen::array<int, 1> reduction_dims = {dim};

    // Find the maximum value along the specified dimension
    auto x_max = x.maximum(reduction_dims);

    // Broadcasting the maximum value for numerical stability in log-sum-exp
    auto x_max_broadcasted = x_max.reshape(Eigen::array<Eigen::Index, 2>{x.dimension(0), 1})
                                    .broadcast(Eigen::array<Eigen::Index, 2>{1, x.dimension(1)});

    // Compute exp(x - x_max) in a numerically stable way
    auto exp_values = (x - x_max_broadcasted).exp();

    // Compute the log sum exp
    auto sum_exp = exp_values.sum(reduction_dims);

    // Compute the log of the sum and final result
    y = (x_max + sum_exp.log()).reshape(y.dimensions());
}

// Explicit instantiation
template void logsumexp<float>(const Eigen::Tensor<float, 2>&, Eigen::Tensor<float, 1>&, const int);
