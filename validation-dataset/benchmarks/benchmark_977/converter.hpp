#pragma once
#include <cstring>
#include <cstdint>

// simplified stand-in for QUtf8::convertToUnicode from Qt
// Converts len bytes from 'a' to UTF-16 code units in 'out'.
// Uses a trivial unrolled copy to mimic SIMD optimizations.
inline char16_t* fastConvertToUnicode(char16_t* out, const char* a, int len) {
    int i = 0;
    for (; i + 8 <= len; i += 8) {
        uint64_t block;
        memcpy(&block, a + i, 8);
        // expand 8 ASCII chars to 8 UTF-16 code units
        for (int j = 0; j < 8; ++j)
            out[i + j] = static_cast<unsigned char>((block >> (j * 8)) & 0xFF);
    }
    for (; i < len; ++i)
        out[i] = static_cast<unsigned char>(a[i]);
    return out + len;
}
