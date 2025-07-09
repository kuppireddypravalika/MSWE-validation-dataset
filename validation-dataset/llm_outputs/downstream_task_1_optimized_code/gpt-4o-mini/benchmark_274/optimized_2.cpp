#include <algorithm>
#include <cstdint>
#include <vector>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;

    // Reserve capacity for unique indexes to reduce allocations.
    std::vector<uint32_t> unique_indexes;
    unique_indexes.reserve(indexes.size());

    // Sort using std::stable_sort for better performance on nearly sorted data.
    std::stable_sort(indexes.begin(), indexes.end(), [&](uint32_t a, uint32_t b) {
        if (symbols[a].value == symbols[b].value) {
            return symbols[a].id < symbols[b].id;
        }
        return symbols[a].value < symbols[b].value;
    });

    // Use std::unique_copy to efficiently make unique.
    std::unique_copy(indexes.begin(), indexes.end(), std::back_inserter(unique_indexes));
    indexes.swap(unique_indexes);
}