#include <algorithm>
#include <cstdint>
#include <vector>
#include <unordered_set>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;

    // Reserve space to maintain unique indexes
    std::unordered_set<uint32_t> unique_indexes;
    unique_indexes.reserve(indexes.size());

    // Insert unique indexes into the unordered_set
    for (const auto& index : indexes) {
        unique_indexes.insert(index);
    }

    // Clear indexes and reserve space for the unique ones
    indexes.clear();
    indexes.reserve(unique_indexes.size());

    // Move unique indexes back into the vector
    indexes.insert(indexes.end(), unique_indexes.begin(), unique_indexes.end());

    // Sort the symbols by value and then by id
    std::sort(indexes.begin(), indexes.end(), [&symbols](uint32_t a, uint32_t b) {
        if (symbols[a].value == symbols[b].value)
            return symbols[a].id < symbols[b].id;
        return symbols[a].value < symbols[b].value;
    });
}