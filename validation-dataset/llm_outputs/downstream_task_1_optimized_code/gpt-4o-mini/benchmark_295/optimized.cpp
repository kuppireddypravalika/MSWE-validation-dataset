#include <algorithm>
#include <vector>
#include <cstdint>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

// Inlined comparator for performance optimization
inline bool attr_less(const Attribute& a, const Attribute& b) {
    return (a.kind != b.kind) ? (a.kind < b.kind) : (a.value < b.value);
}

unsigned long sort_attributes(const std::vector<Attribute>& attrs) {
    if (attrs.empty()) return 0;
    // Use a temporary vector with reserve to prevent unnecessary reallocations
    std::vector<Attribute> sorted;
    sorted.reserve(attrs.size());
    sorted.assign(attrs.begin(), attrs.end());
    std::sort(sorted.begin(), sorted.end(), attr_less);
    unsigned long sum = 0;
    for (const auto& A : sorted) {
        sum += static_cast<unsigned long>(A.kind) + A.value;
    }
    return sum;
}

// Keep the explicit template instantiation statements unchanged
