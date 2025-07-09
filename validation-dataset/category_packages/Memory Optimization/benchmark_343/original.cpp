#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

// Simulated prepare function from PaddlePaddle before the patch
int prepare_op(OperatorWithKernel& op, const AttributeMap& new_attrs) {
#ifdef PADDLE_WITH_MKLDNN
    // Always copy attributes even if MKL-DNN is not used
    auto& mutable_attrs = const_cast<AttributeMap&>(op.Attrs());
    mutable_attrs = new_attrs;
#endif
    int sum = 0;
    for (const auto& kv : op.Attrs()) sum += kv.second;
    return sum;
}
