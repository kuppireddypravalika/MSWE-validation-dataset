#include <cstdint>
#include <vector>

static inline uint32_t generic_bswap_32(uint32_t x) {
    return (x >> 24) |
           ((x >> 8) & 0x0000ff00u) |
           ((x << 8) & 0x00ff0000u) |
           (x << 24);
}

uint64_t process(const std::vector<uint32_t>& data) {
    uint64_t sum = 0;
    for (uint32_t v : data)
        sum += generic_bswap_32(v);
    return sum;
}

// Explicit template instantiation statements preserved
// No explicit template instantiation statements provided in the original code.