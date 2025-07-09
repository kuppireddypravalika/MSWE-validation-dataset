#include <cstdint>
#include <vector>

static inline uint32_t generic_bswap_32(uint32_t x) {
    return (x >> 24) |
           ((x & 0x00ff0000u) >> 8) |
           ((x & 0x0000ff00u) << 8) |
           (x << 24);
}

uint64_t process(const std::vector<uint32_t>& data) {
    uint64_t sum = 0;
    for (uint32_t v : data) {
        sum += generic_bswap_32(v);
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here