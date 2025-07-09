#include <cstdint>
#include <cstddef>

// Optimized bitwise select without the extra copy
// dst[i] = (mask[i] & true_v[i]) | (~mask[i] & false_v[i])

extern "C" uint64_t vbsl(const uint64_t* mask,
                                    const uint64_t* true_v,
                                    const uint64_t* false_v,
                                    uint64_t* dst,
                                    size_t n) {
    for (size_t i = 0; i < n; ++i)
        dst[i] = (true_v[i] & mask[i]) | (false_v[i] & ~mask[i]);
    uint64_t sum = 0;
    for (size_t i = 0; i < n; ++i)
        sum += dst[i];
    return sum;
}
