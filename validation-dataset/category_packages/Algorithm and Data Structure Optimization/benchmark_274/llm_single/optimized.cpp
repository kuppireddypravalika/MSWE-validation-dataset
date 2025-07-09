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
    // Reserve space in advance using a lambda for improved locality and reduced overhead.
    std::vector<std::pair<uint64_t, uint32_t>> symbol_pairs;
    symbol_pairs.reserve(indexes.size());
    for (uint32_t index : indexes) {
        symbol_pairs.emplace_back(symbols[index].value, symbols[index].id);
    }
    // Sort using a custom comparator.
    std::sort(indexes.begin(), indexes.end(), [&symbol_pairs](uint32_t a, uint32_t b) {
        return symbol_pairs[a] < symbol_pairs[b];
    });
    // Use std::unique in conjunction with erase for direct removal of duplicates.
    auto last = std::unique(indexes.begin(), indexes.end());
    indexes.erase(last, indexes.end());
}
