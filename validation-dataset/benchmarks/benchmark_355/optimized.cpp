#include <vector>
#include <cstdint>
#include <cstring>
#include <cassert>

struct MockPLT {
    std::vector<int> entries;
    explicit MockPLT(size_t n) : entries(n) {
        for (size_t i = 0; i < n; ++i) entries[i] = static_cast<int>(i);
    }
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
    assert(plt && "emitSectionData failed, m_pPLT is NULL!");
    // Optimized version calls the fix-ups only once before copying
    plt->applyPLT0();
    plt->applyPLT1();
    for (size_t i = 0; i < n; ++i) buffer[i] = plt->entries[i];
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
