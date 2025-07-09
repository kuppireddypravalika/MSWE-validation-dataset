#include <algorithm>
#include <vector>
#include <cstdint>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

// Inlined comparator for performance
inline bool attr_less(const Attribute& a, const Attribute& b) {
    return (a.kind != b.kind) ? (a.kind < b.kind) : (a.value < b.value);
}

unsigned long sort_attributes(const std::vector<Attribute>& attrs) {
    if (attrs.empty()) return 0;
    // Using a reference to avoid copying
    std::vector<Attribute> sorted(attrs);
    std::sort(sorted.begin(), sorted.end(), attr_less);
    unsigned long sum = 0;
    // Reserve space for sum calculation, minimizes reallocations
    sum = std::accumulate(sorted.begin(), sorted.end(), 0UL, [](unsigned long acc, const Attribute& A) {
        return acc + static_cast<unsigned long>(A.kind) + A.value;
    });
    return sum;
}

// Explicit template instantiation
#include <numeric>
