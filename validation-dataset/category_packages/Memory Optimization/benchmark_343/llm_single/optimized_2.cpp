#include <unordered_map>
#include <string>

using AttributeMap = std::unordered_map<std::string, int>;

struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

// Optimized prepare function from PaddlePaddle
int prepare_op(int size, int repeat) {
    // Preserving the same behavior and functionality
    AttributeMap new_attrs;
    new_attrs.reserve(size);
    for (int i = 0; i < size; ++i) {
        new_attrs["attr_" + std::to_string(i)] = repeat;
    }

    int sum = 0;
    for (const auto& kv : new_attrs) {
        sum += kv.second;
    }
    return sum;
}

// Explicit template instantiation statements
