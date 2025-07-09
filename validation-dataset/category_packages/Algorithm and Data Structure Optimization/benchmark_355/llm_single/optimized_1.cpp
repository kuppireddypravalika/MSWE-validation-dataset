#include <vector>
#include <cstdint>
#include <cstring>

struct MockPLT {
    std::vector<int> entries;
    explicit MockPLT(size_t n) : entries(n) {
        for (size_t i = 0; i < n; ++i) entries[i] = static_cast<int>(i);
    }
    // Heavy but pure helper routines
    void applyPLT() const {
        volatile int tmp = 0;
        for (size_t i = 0; i < entries.size(); ++i) tmp += entries[i];
    }
};

static uint64_t emitSectionData(const MockPLT* plt, int* buffer, size_t n) {
    // Reduce redundant calls to applyPLT0 and applyPLT1 into a single call
    plt->applyPLT();
    std::memcpy(buffer, plt->entries.data(), n * sizeof(int));
    uint64_t sum = 0;
    for (size_t i = 0; i < n; ++i) sum += buffer[i];
    return sum;
}

uint64_t run(size_t entries, size_t loops) {
    MockPLT plt(entries);
    std::vector<int> buffer(entries);
    uint64_t total = 0;
    for (size_t i = 0; i < loops; ++i) total += emitSectionData(&plt, buffer.data(), entries);
    return total;
}

// Explicit template instantiation statements
