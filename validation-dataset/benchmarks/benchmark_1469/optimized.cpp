#include "simd_utils.hpp"

// Optimized VectorGetLow implementation: shift left then right
Vec VectorGetLow(const Vec& val) {
    return VectorShiftRight<8>(VectorShiftLeft<8>(val));
}
