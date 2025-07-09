#include <cstdint>
#include <cstddef>

// Optimized implementation of vbsl using vectorized operations

extern "C" uint64_t vbsl(const uint64_t* mask,
                                   const uint64_t* true_v,
                                   const uint64_t* false_v,
                                   uint64_t* dst,
                                   size_t n) {
    uint64_t sum = 0;
    size_t i = 0;

    // Unroll loop to minimize bounds checking and maximize performance
    for (; i + 4 <= n; i += 4) {
        // Compute results directly in dst based on the mask
        dst[i]     = (mask[i]     & true_v[i])     | (~mask[i]     & false_v[i]);
        dst[i + 1] = (mask[i + 1] & true_v[i + 1]) | (~mask[i + 1] & false_v[i + 1]);
        dst[i + 2] = (mask[i + 2] & true_v[i + 2]) | (~mask[i + 2] & false_v[i + 2]);
        dst[i + 3] = (mask[i + 3] & true_v[i + 3]) | (~mask[i + 3] & false_v[i + 3]);
    }

    // Handle any remaining elements
    for (; i < n; ++i) {
        dst[i] = (mask[i] & true_v[i]) | (~mask[i] & false_v[i]);
    }

    // Calculate sum in a separate pass to avoid doing it per iteration
    for (i = 0; i < n; ++i) {
        sum += dst[i];
    }

    return sum;
}