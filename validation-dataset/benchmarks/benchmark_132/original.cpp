#include <cstdint>
#include <vector>

// Naive byte swap without compiler optimizations. On IA-32 this was the
// original implementation before inline assembly was introduced.
__attribute__((optimize("O0")))
static inline uint32_t generic_bswap_32(uint32_t x) {
    return ((x & 0xff000000u) >> 24) |
           ((x & 0x00ff0000u) >> 8)  |
           ((x & 0x0000ff00u) << 8)  |
           ((x & 0x000000ffu) << 24);
}

uint64_t process(const std::vector<uint32_t>& data) {
    uint64_t sum = 0;
    for (uint32_t v : data)
        sum += generic_bswap_32(v);
    return sum;
}
