#include <algorithm>
#include <vector>
#include <cstdint>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

// Non-inlined comparator similar to LLVM's Attribute operator<
__attribute__((noinline)) bool attr_less(const Attribute& a, const Attribute& b) {
    if (a.kind != b.kind)
        return a.kind < b.kind;
    return a.value < b.value;
}

unsigned long sort_attributes(const std::vector<Attribute>& attrs) {
    if (attrs.empty()) return 0;
    std::vector<Attribute> sorted(attrs.begin(), attrs.end());
    std::sort(sorted.begin(), sorted.end(), attr_less);
    unsigned long sum = 0;
    for (const auto& A : sorted)
        sum += static_cast<unsigned long>(A.kind) + A.value;
    return sum;
}
