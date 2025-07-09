#include <vector>
#include <cstdint>
#include <cstddef>

// Optimized conversion that writes directly to the destination
// without an intermediate temporary buffer.

void convert_half_to_float(std::vector<float>& dst, const std::vector<uint16_t>& src) {
    for (std::size_t i = 0; i < src.size(); ++i) {
        dst[i] = static_cast<float>(src[i]);
    }
}
