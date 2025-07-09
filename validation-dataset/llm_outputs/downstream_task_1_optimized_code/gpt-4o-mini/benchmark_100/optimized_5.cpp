#include <cstdint>
#include <cstring>

constexpr int ChunkBits = 10;
constexpr int PageBits  = 10;
constexpr int PageSize  = 1 << PageBits;
constexpr size_t PageChunkMask = (1ULL << (ChunkBits + PageBits)) - 1;

static inline void write_byte(uint8_t* page, size_t b0, uint8_t val)
{
    size_t c0 = b0 >> ChunkBits;
    page[c0] = val; // Directly set value instead of using memset
}

uint64_t run(size_t loops)
{
    uint8_t page[PageSize] = {};
    uint64_t sum = 0;
    for(size_t i = 0; i < loops; i++)
    {
        size_t b0 = (i * 13) & (PageChunkMask - 1);
        write_byte(page, b0, 1);
        sum += page[b0 >> ChunkBits];
    }
    return sum;
}

// Explicit template instantiation statements (if they exist)
