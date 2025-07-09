#include <cstdint>

static inline unsigned getMinLeadingZeros(uint32_t x, unsigned bits) {
    unsigned count = 0;
    for (int i = bits - 1; i >= 0; --i) {
        if (x & (1u << i)) break;
        ++count;
    }
    return count;
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
