#include <algorithm>
#include <vector>
#include <cstdint>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

// Simplified array_pod_sort specialized for Attribute (POD)
template <typename Iter>
inline void array_pod_sort(Iter begin, Iter end) {
    std::sort(begin, end, [](const Attribute& a, const Attribute& b) {
        uint64_t av = (static_cast<uint64_t>(a.kind) << 32) | a.value;
        uint64_t bv = (static_cast<uint64_t>(b.kind) << 32) | b.value;
        return av < bv;
    });
}

unsigned long sort_attributes(const std::vector<Attribute>& attrs) {
    if (attrs.empty()) return 0;
    std::vector<Attribute> sorted(attrs.begin(), attrs.end());
    array_pod_sort(sorted.begin(), sorted.end());
    unsigned long sum = 0;
    for (const auto& A : sorted)
        sum += static_cast<unsigned long>(A.kind) + A.value;
    return sum;
}
