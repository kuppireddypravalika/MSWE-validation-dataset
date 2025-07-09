#include <cstdint>
#include <cstring>

constexpr int ChunkBits = 10;
constexpr int PageBits  = 10;
constexpr int PageSize  = 1 << PageBits;
constexpr size_t PageChunkMask = (1ULL << (ChunkBits + PageBits)) - 1;

static inline void write_byte(uint8_t* page, size_t b0, size_t b1, uint8_t val)
{
    size_t c0 = b0 >> ChunkBits;
    size_t c1 = (b1 >> ChunkBits) + 1;
    memset(page + c0, val, c1 - c0);
}

uint64_t run(size_t loops)
{
    uint8_t page[PageSize] = {};
    uint64_t sum = 0;
    size_t b_offset = (loops * 13) & (PageChunkMask - 1);
    for(size_t i = 0; i < loops; i++)
    {
        size_t b0 = (b_offset + i * 13) & (PageChunkMask - 1);
        write_byte(page, b0, b0 + 1, 1);
        sum += page[b0 >> ChunkBits];
    }
    return sum;
}

// Explicit template instantiation statements if any exist in original
