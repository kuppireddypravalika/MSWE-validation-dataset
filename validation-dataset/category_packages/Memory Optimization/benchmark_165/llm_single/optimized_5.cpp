#include <cstdint>
#include <cstddef>

// Perform bitwise select using a single loop
// dst[i] = (mask[i] & true_v[i]) | (~mask[i] & false_v[i])
// using bitwise operations on a single pass

extern "C" uint64_t vbsl(const uint64_t* mask,
                                   const uint64_t* true_v,
                                   const uint64_t* false_v,
                                   uint64_t* dst,
                                   size_t n) {
    uint64_t sum = 0;
    for (size_t i = 0; i < n; ++i) {
        // Compute selected value based on mask
        dst[i] = (mask[i] & true_v[i]) | (false_v[i] & ~mask[i]);
        // Accumulate sum
        sum += dst[i];
    }
    return sum;
}
