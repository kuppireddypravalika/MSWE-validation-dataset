#include "simd_utils.hpp"

// Original VectorGetLow implementation: shift right then left
Vec VectorGetLow(const Vec& val) {
    return VectorShiftLeft<8>(VectorShiftRight<8>(val));
}
