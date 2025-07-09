#include <cstdint>
#include <cstring>
#include <array>

constexpr int ChunkBits = 10;
constexpr int PageBits  = 10;
constexpr int PageSize  = 1 << PageBits;
constexpr size_t PageChunkMask = (1ULL << (ChunkBits + PageBits)) - 1;

uint64_t run(size_t loops) {
    std::array<uint8_t, PageSize> page{};
    uint64_t sum = 0;

    for(size_t i = 0; i < loops; i++) {
        size_t b0 = (i * 13) & (PageChunkMask - 1);
        page[b0] = 1; // Write byte directly
        sum += page[b0 >> ChunkBits];
    }

    return sum;
}

// Explicit template instantiation statements, unchanged
