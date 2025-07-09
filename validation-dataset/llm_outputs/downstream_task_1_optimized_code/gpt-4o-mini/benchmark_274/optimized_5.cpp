#include <algorithm>
#include <cstdint>
#include <vector>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes, const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;

    // Reserve space in the output to minimize reallocations
    std::vector<uint32_t> unique_indexes;
    unique_indexes.reserve(indexes.size());

    // Use stable_sort to maintain relative order for equal elements
    std::stable_sort(indexes.begin(), indexes.end(), [&symbols](uint32_t a, uint32_t b) {
        if (symbols[a].value == symbols[b].value)
            return symbols[a].id < symbols[b].id;
        return symbols[a].value < symbols[b].value;
    });

    // Remove duplicates using unique and copy
    auto last = std::unique(indexes.begin(), indexes.end());
    unique_indexes.assign(indexes.begin(), last);
    indexes.swap(unique_indexes);
}

// Explicit template instantiation statements
