#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

int prepare_op(int size, int repeat) {
#ifdef PADDLE_WITH_MKLDNN
    // Assuming preparation of new_attrs happens here if necessary
    // This code simulates behavior without excessive references
    static AttributeMap new_attrs;
    new_attrs.reserve(size);
    for (int i = 0; i < size; ++i) {
        new_attrs[std::to_string(i)] = repeat;
    }
    // Always copy attributes even if MKL-DNN is not used
    auto& mutable_attrs = const_cast<AttributeMap&>(op.Attrs());
    mutable_attrs = std::move(new_attrs);
#endif
    int sum = 0;
    for (const auto& kv : op.Attrs()) {
        sum += kv.second;
    }
    return sum;
}
