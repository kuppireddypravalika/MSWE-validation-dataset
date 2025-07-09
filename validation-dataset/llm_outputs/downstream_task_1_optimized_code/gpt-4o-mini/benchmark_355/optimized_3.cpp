#include <vector>
#include <cstdint>
#include <cstring>

struct MockPLT {
    std::vector<int> entries;
    explicit MockPLT(size_t n) : entries(n) {
        for (size_t i = 0; i < n; ++i) entries[i] = static_cast<int>(i);
    }
    // Heavy but pure helper routines
    void applyPLT0() const {
        volatile int tmp = 0;
        for (size_t i = 0; i < entries.size(); ++i) tmp += entries[i];
    }
    void applyPLT1() const {
        volatile int tmp = 0;
        for (size_t i = 1; i < entries.size(); ++i) tmp += entries[i];
    }
};

static uint64_t emitSectionData(const MockPLT* plt, int* buffer, size_t n) {
    // Optimize by reducing redundant calls to applyPLT0 and applyPLT1
    plt->applyPLT0();
    plt->applyPLT1();
    std::memcpy(buffer, plt->entries.data(), n * sizeof(int)); // more efficient copy
    return std::accumulate(buffer, buffer + n, uint64_t{0}); // use accumulate for summation
}

uint64_t run(size_t entries, size_t loops) {
    MockPLT plt(entries);
    std::vector<int> buffer(entries);
    uint64_t total = 0;
    // Use a single loop and accumulate over the number of loops
    for (size_t i = 0; i < loops; ++i) total += emitSectionData(&plt, buffer.data(), entries);
    return total;
}

// Explicit template instantiation statements below (if any)
