#include <cstdint>
#include <vector>

// Optimized byte swap using the x86 bswap instruction as introduced
// in commit 9da81c79a2c6c91950b316fd89eeeac5b26f0d93.
static inline uint32_t opt_bswap_32(uint32_t x) {
    uint32_t v;
    __asm__("bswap %0" : "=r"(v) : "0"(x));
    return v;
}

uint64_t process(const std::vector<uint32_t>& data) {
    uint64_t sum = 0;
    for (uint32_t v : data)
        sum += opt_bswap_32(v);
    return sum;
}
