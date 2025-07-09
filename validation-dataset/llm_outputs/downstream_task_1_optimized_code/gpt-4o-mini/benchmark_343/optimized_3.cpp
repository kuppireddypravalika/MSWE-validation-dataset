#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

int prepare_op(int size, int repeat) {
    OperatorWithKernel op;
    AttributeMap new_attrs(size);

    // Initialize new_attrs directly with dummy data to avoid repeated allocations
    for (int i = 0; i < size; ++i) {
        new_attrs[std::to_string(i)] = repeat;
    }

#ifdef PADDLE_WITH_MKLDNN
    auto& mutable_attrs = const_cast<AttributeMap&>(op.Attrs());
    mutable_attrs = std::move(new_attrs);  // Use move to avoid copies
#endif

    int sum = 0;
    for (const auto& kv : op.Attrs()) sum += kv.second;
    return sum;
}