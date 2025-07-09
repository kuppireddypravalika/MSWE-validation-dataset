#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

// Global flag controlling MKL-DNN usage
extern bool FLAGS_use_mkldnn;

// Optimized prepare function that copies only when MKL-DNN is enabled
int prepare_op(OperatorWithKernel& op, const AttributeMap& new_attrs) {
#ifdef PADDLE_WITH_MKLDNN
    if (FLAGS_use_mkldnn) {
        auto& mutable_attrs = const_cast<AttributeMap&>(op.Attrs());
        mutable_attrs = new_attrs;
    }
#endif
    int sum = 0;
    for (const auto& kv : op.Attrs()) sum += kv.second;
    return sum;
}
