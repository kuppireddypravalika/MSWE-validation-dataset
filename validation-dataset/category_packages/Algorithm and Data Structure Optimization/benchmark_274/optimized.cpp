#include <algorithm>
#include <cstdint>
#include <vector>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

struct SymbolIndexComparator {
    const std::vector<Symbol>& symbols;
    bool operator()(uint32_t a, uint32_t b) const {
        if (symbols[a].value == symbols[b].value)
            return symbols[a].id < symbols[b].id;
        return symbols[a].value < symbols[b].value;
    }
};

void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols) {
    if (indexes.size() <= 1)
        return;
    // Use std::stable_sort which performs better for nearly sorted input
    std::stable_sort(indexes.begin(), indexes.end(), SymbolIndexComparator{symbols});
    indexes.erase(std::unique(indexes.begin(), indexes.end()), indexes.end());
}
