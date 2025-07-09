#include <algorithm>
#include <cstdint>
#include <vector>
#include <unordered_map>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;

    // Using a stable sort to preserve the relative order of equal elements without going through unique()
    std::stable_sort(indexes.begin(), indexes.end(), [&symbols](uint32_t a, uint32_t b) {
        return std::tie(symbols[a].value, symbols[a].id) < std::tie(symbols[b].value, symbols[b].id);
    });

    // Remove duplicates in the sorted indexes.
    auto last = std::unique(indexes.begin(), indexes.end());
    indexes.erase(last, indexes.end());
}

// Explicit template instantiation statements
