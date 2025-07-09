
#include "conv_common.hpp"

namespace bench633 {

template <std::size_t D>
struct conv_apply_original {
    template <typename A, typename B, typename C>
    static void apply(A&& a, B&& b, C&& c) {
        conv2d_valid_impl::apply(a, b, c);
    }
};

void do_conv(const AddMatrixExpr& a, const AddMatrixExpr& b, Matrix& c) {
    conv_apply_original<2>::apply(a, b, c);
}

} // namespace bench633
