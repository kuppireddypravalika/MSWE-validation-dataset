#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

using sint32 = int32_t;
using uint8 = uint8_t;
using uint16 = uint16_t;

struct rct_drawpixelinfo {
    sint32 width;
    sint32 height;
    sint32 pitch;
    sint32 x;
    sint32 y;
    sint32 zoom_level;
    uint8* bits;
};

struct rct_g1_element {
    uint8* offset;
};

inline uint8 g1_palette[256];
inline rct_g1_element g1Elements[1];
inline uint16 palette_to_g1_offset[1] = {0};

inline void init_g1() {
    for (int i = 0; i < 256; ++i) g1_palette[i] = static_cast<uint8>(255 - i);
    g1Elements[0].offset = g1_palette;
}
