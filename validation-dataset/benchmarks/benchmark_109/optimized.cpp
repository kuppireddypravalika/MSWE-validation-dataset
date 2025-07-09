#include <vector>
#include <cstdint>
#include <cstring>

namespace bench109 {

// same constants and structs as in original
enum { ChunkBits = 10 };
enum { PageBits = 10 };
enum { PageSize = 1 << PageBits };
enum { PageChunkBits = ChunkBits + PageBits };
enum { PageChunkSize = 1 << PageChunkBits };
enum { PageChunkMask = PageChunkSize - 1 };

struct Alloc {
    uint64_t ptr;
    uint64_t size;
    int64_t timeAlloc;
    int64_t timeFree;
};

struct MemData {
    std::vector<Alloc> data;
    uint64_t low;
    uint64_t high;
};

static inline void PreparePage(std::vector<int8_t>& page)
{
    if (page.empty()) {
        page.resize(PageSize);
        memset(page.data(), 0, PageSize);
    }
}

std::vector<std::vector<int8_t>> getMemoryPages(const MemData& mem,
                                                          bool restrictTime,
                                                          uint64_t zvStart,
                                                          uint64_t zvEnd)
{
    std::vector<std::vector<int8_t>> ret;
    const auto span = mem.high - mem.low;
    const auto pages = (span / PageChunkSize) + 1;
    ret.resize(pages);

    const auto zvMid = zvStart + (zvEnd - zvStart) / 2;

    for (const auto& alloc : mem.data) {
        if (restrictTime && alloc.timeAlloc > zvMid) continue;

        const auto a0 = alloc.ptr - mem.low;
        const auto a1 = a0 + alloc.size;
        const auto p0 = a0 >> PageChunkBits;
        const auto p1 = a1 >> PageChunkBits;

        int8_t val = alloc.timeFree < 0 ? 1 : (restrictTime ? (alloc.timeFree > zvMid ? 1 : -1) : -1);

        if (p0 == p1) {
            auto& page = ret[p0];
            PreparePage(page);
            const auto b0 = a0 & PageChunkMask;
            const auto b1 = a1 & PageChunkMask;
            const auto c0 = b0 >> ChunkBits;
            const auto c1 = b1 >> ChunkBits;
            if (c0 == c1) {
                *(page.data() + c0) = val;
            } else {
                memset(page.data() + c0, val, c1 - c0 + 1);
            }
        } else {
            {
                auto& page = ret[p0];
                PreparePage(page);
                const auto b0 = a0 & PageChunkMask;
                const auto c0 = b0 >> ChunkBits;
                memset(page.data() + c0, val, PageSize - c0);
            }
            for (uint64_t i = p0 + 1; i < p1; ++i) {
                auto& page = ret[i];
                if (page.empty()) page.resize(PageSize);
                memset(page.data(), val, PageSize);
            }
            {
                auto& page = ret[p1];
                PreparePage(page);
                const auto b1 = a1 & PageChunkMask;
                const auto c1 = (b1 >> ChunkBits) + 1;
                memset(page.data(), val, c1);
            }
        }
    }
    return ret;
}

} // namespace bench109
