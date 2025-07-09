#include <algorithm>
#include <cstdint>
#include <vector>
#include <memory>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;

    // Reserve space for unique elements if possible
    indexes.reserve(std::distance(indexes.begin(), std::unique(indexes.begin(), indexes.end())));

    // Sort using a custom comparator lambda for better inlining and performance
    std::sort(indexes.begin(), indexes.end(), [&symbols](uint32_t a, uint32_t b) {
        return symbols[a].value == symbols[b].value ? symbols[a].id < symbols[b].id : symbols[a].value < symbols[b].value;
    });

    // Erase duplicates while maintaining stability
    auto last = std::unique(indexes.begin(), indexes.end(), [&](uint32_t a, uint32_t b) {
        return symbols[a].value == symbols[b].value && symbols[a].id == symbols[b].id;
    });
    indexes.erase(last, indexes.end());
}

// Explicit template instantiation statements (if any) will go here.