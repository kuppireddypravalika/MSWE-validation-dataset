// optimized.cpp
#include <cstdint>
#include <cstddef>

extern "C" uint64_t vbsl(const uint64_t* mask,
                                   const uint64_t* true_v,
                                   const uint64_t* false_v,
                                   uint64_t* dst,
                                   size_t n) {
    uint64_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        // Combine the mask and values in a single pass
        dst[i] = (mask[i] & true_v[i]) | (false_v[i] & ~mask[i]);
        sum += dst[i];
    }
    return sum;
}

// Explicit template instantiation statements
