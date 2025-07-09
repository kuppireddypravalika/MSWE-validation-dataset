#include <cstdint>
#include <cstring>

constexpr int ChunkBits = 10;
constexpr int PageBits  = 10;
constexpr int PageSize  = 1 << PageBits;
constexpr size_t PageChunkMask = (1ULL << (ChunkBits + PageBits)) - 1;

static inline void write_byte(uint8_t* page, size_t b0, size_t b1, uint8_t val)
{
    size_t c0 = b0 >> ChunkBits;
    size_t c1 = b1 >> ChunkBits;
    if(c0 == c1)
    {
        *(page + c0) = val;
    }
    else
    {
        memset(page + c0, val, c1 - c0 + 1);
    }
}

uint64_t run(size_t loops)
{
    uint8_t page[PageSize] = {};
    uint64_t sum = 0;
    for(size_t i=0;i<loops;i++)
    {
        size_t b0 = (i*13) & (PageChunkMask-1);
        size_t b1 = b0 + 1; // one byte region
        write_byte(page, b0, b1, 1);
        sum += page[b0 >> ChunkBits];
    }
    return sum;
}
