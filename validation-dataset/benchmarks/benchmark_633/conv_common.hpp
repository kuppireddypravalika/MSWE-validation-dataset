#ifndef BENCH633_CONV_COMMON_HPP
#define BENCH633_CONV_COMMON_HPP

#include <vector>
#include <cstddef>

namespace bench633 {

// Basic matrix container with contiguous storage
class Matrix {
    std::size_t r_, c_;
    std::vector<double> data_;
public:
    Matrix(std::size_t r=0, std::size_t c=0) : r_(r), c_(c), data_(r*c) {}
    std::size_t rows() const { return r_; }
    std::size_t cols() const { return c_; }
    double& operator()(std::size_t i, std::size_t j) { return data_[i*c_ + j]; }
    double operator()(std::size_t i, std::size_t j) const { return data_[i*c_ + j]; }
    Matrix evaluate() const { return *this; }
};

// Expression that computes element-wise sum of two matrices lazily
class AddMatrixExpr {
    const Matrix& A_; const Matrix& B_;
public:
    AddMatrixExpr(const Matrix& A, const Matrix& B) : A_(A), B_(B) {}
    std::size_t rows() const { return A_.rows(); }
    std::size_t cols() const { return A_.cols(); }
    double operator()(std::size_t i, std::size_t j) const { return A_(i,j) + B_(i,j); }
    Matrix evaluate() const {
        Matrix tmp(rows(), cols());
        for (std::size_t i=0;i<rows();++i)
            for (std::size_t j=0;j<cols();++j)
                tmp(i,j) = (*this)(i,j);
        return tmp;
    }
};

// create a temporary matrix from an expression
template <typename Expr>
Matrix make_temporary(const Expr& e) {
    return e.evaluate();
}

// Convolution implementation (valid, 2D)
struct conv2d_valid_impl {
    template <typename In, typename Ker, typename Out>
    static void apply(const In& input, const Ker& kernel, Out& output) {
        const std::size_t ir = input.rows();
        const std::size_t ic = input.cols();
        const std::size_t kr = kernel.rows();
        const std::size_t kc = kernel.cols();
        for (std::size_t i=0;i<=ir-kr;++i) {
            for (std::size_t j=0;j<=ic-kc;++j) {
                double sum=0.0;
                for (std::size_t r=0;r<kr;++r)
                    for (std::size_t c=0;c<kc;++c)
                        sum += input(i+r,j+c)*kernel(r,c);
                output(i,j)=sum;
            }
        }
    }
};

} // namespace bench633

#endif
