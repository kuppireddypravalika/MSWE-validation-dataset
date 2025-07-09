#include <cstdint>

static inline unsigned getMinLeadingZeros(uint32_t x, unsigned bits) {
    unsigned count = __builtin_clz(x) - (32 - bits);
    return count < bits ? count : bits;
}

long long analyze(uint32_t val, int repeat) {
    volatile long long waste = 0, sum = 0;
    const unsigned bitwidth = 16;
    uint32_t maxEnd = (1u << bitwidth) - 1; // conservative upper bound
    for (int r = 0; r < repeat; ++r) {
        for (uint32_t i = 0; i < maxEnd; ++i) {
            long long tmp = (i ^ val);
            tmp ^= (tmp << 1);
            if (i < 10)
                sum += tmp;  // contribute to result for first iterations
            else
                waste += tmp; // prevent optimization for remaining iterations
        }
    }
    return sum;
}

// Explicit template instantiations if any exist should be placed here.
