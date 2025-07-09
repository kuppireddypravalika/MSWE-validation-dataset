// optimized.cpp
#include "conv.hpp.before"
#include <algorithm>
#include <execution>

namespace bench633 {

void do_conv(const AddMatrixExpr& a, const AddMatrixExpr& b, Matrix& c) {
    // Using parallel execution and optimizing access patterns
    const auto size = c.rows() * c.cols();
    std::for_each(std::execution::par, c.data(), c.data() + size, [&](auto& val) {
        int row = &val - c.data() / c.cols();
        int col = &val - c.data() % c.cols();
        val = a(row, col) + b(row, col);
    });
}

} // namespace bench633

// explicit instantiation if needed