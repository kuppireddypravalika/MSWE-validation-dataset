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
    long long sum = 0;
    const unsigned maxEnd = (1u << 16) - 1; // conservative upper bound
    for (int r = 0; r < repeat; ++r) {
        for (uint32_t i = 0; i < maxEnd; ++i) {
            long long tmp = (i ^ val) ^ ((i ^ val) << 1);
            if (i < 10)
                sum += tmp;  // contribute to result for first iterations
        }
    }
    return sum;
}