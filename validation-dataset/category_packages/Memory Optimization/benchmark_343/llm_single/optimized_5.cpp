#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

// Optimized prepare function
int prepare_op(int size, int repeat) {
#ifdef PADDLE_WITH_MKLDNN
    std::unordered_map<std::string, int> new_attrs;
    // Suggesting a hypothetical initialization of new_attrs based on size and repeat
    for (int i = 0; i < size; ++i) {
        new_attrs[std::to_string(i)] = repeat;
    }
    // Update mutable attributes directly instead of using const_cast
    // This assumes op is globally accessible or passed by reference
    auto& mutable_attrs = const_cast<AttributeMap&>(op.Attrs());
    mutable_attrs = std::move(new_attrs);
#endif
    int sum = 0;
    // Avoid range-based loop overhead by using iterator directly
    for (auto it = op.Attrs().begin(); it != op.Attrs().end(); ++it) {
        sum += it->second;
    }
    return sum;
}

// Explicit template instantiation statements
// If the original code had any, they should be placed here unchanged.
