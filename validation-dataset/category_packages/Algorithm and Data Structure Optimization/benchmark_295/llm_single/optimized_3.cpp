#include <algorithm>
#include <vector>
#include <cstdint>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

// Inline comparator for improved performance
inline bool attr_less(const Attribute& a, const Attribute& b) {
    return (a.kind != b.kind) ? (a.kind < b.kind) : (a.value < b.value);
}

unsigned long sort_attributes(const std::vector<Attribute>& attrs) {
    if (attrs.empty()) return 0;
    // Use a vector's reserve to avoid unnecessary allocations
    std::vector<Attribute> sorted;
    sorted.reserve(attrs.size());
    sorted.insert(sorted.end(), attrs.begin(), attrs.end());
    // Sort using an inline comparator
    std::sort(sorted.begin(), sorted.end(), attr_less);
    unsigned long sum = 0;
    // Preallocate space for result sum calculation
    sum = std::accumulate(sorted.begin(), sorted.end(), 0ul, [](unsigned long acc, const Attribute& A) {
        return acc + static_cast<unsigned long>(A.kind) + A.value;
    });
    return sum;
}
