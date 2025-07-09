#include <vector>
#include <cstdint>
#include <cstddef>

// Convert half-precision values (represented as uint16_t) to floats
// using a temporary buffer, mirroring the original implementation
// that used an extra register.

void convert_half_to_float(std::vector<float>& dst, const std::vector<uint16_t>& src) {
    std::vector<uint16_t> tmp(src.size());
    // zip1 step - interleave elements into the temporary
    for (std::size_t i = 0; i < src.size(); ++i) {
        tmp[i] = src[i];
    }
    // fcvtlt step - widen to 32-bit floats
    for (std::size_t i = 0; i < src.size(); ++i) {
        dst[i] = static_cast<float>(tmp[i]);
    }
}
