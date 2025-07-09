#pragma once
#include <array>
#include <cstdint>

using Vec = std::array<uint8_t,16>;

// Shift vector left by C bytes, filling the low bytes with zeros
template <unsigned C>
inline Vec VectorShiftLeft(const Vec& v) {
    Vec out{};
    if (C >= 16) return out;
    for (unsigned i = 0; i < 16 - C; ++i)
        out[i] = v[i + C];
    return out;
}

// Shift vector right by C bytes, filling the high bytes with zeros
template <unsigned C>
inline Vec VectorShiftRight(const Vec& v) {
    Vec out{};
    if (C >= 16) return out;
    for (unsigned i = 0; i < 16 - C; ++i)
        out[i + C] = v[i];
    return out;
}
