#include "drawing.hpp"

void filter_rect(rct_drawpixelinfo* dpi, int palette, sint32 left, sint32 top, sint32 right, sint32 bottom) {
    if (left > right) return;
    if (top > bottom) return;
    if (dpi->x > right) return;
    if (left >= dpi->x + dpi->width) return;
    if (bottom < dpi->y) return;
    if (top >= dpi->y + dpi->height) return;

    sint32 startX = left - dpi->x;
    if (startX < 0) startX = 0;

    sint32 endX = right - dpi->x + 1;
    if (endX > dpi->width) endX = dpi->width;

    sint32 startY = top - dpi->y;
    if (startY < 0) startY = 0;

    sint32 endY = bottom - dpi->y + 1;
    if (endY > dpi->height) endY = dpi->height;

    sint32 width = endX - startX;
    sint32 height = endY - startY;

    uint8* dst = dpi->bits + (uint32_t)((startY >> dpi->zoom_level) * ((dpi->width >> dpi->zoom_level) + dpi->pitch) + (startX >> dpi->zoom_level));

    uint16 g1Index = palette_to_g1_offset[palette];
    rct_g1_element* g1Element = &g1Elements[g1Index];
    uint8* g1Bits = g1Element->offset;

    for (sint32 i = 0; i < height >> dpi->zoom_level; i++) {
        uint8* nextdst = dst + ((dpi->width >> dpi->zoom_level) + dpi->pitch) * i;
        for (sint32 j = 0; j < (width >> dpi->zoom_level); j++) {
            *(nextdst + j) = g1Bits[*(nextdst + j)];
        }
    }
}

// explicit instantiation like openrct2 style (not necessary but for consistency)
void filter_rect_wrapper(rct_drawpixelinfo* dpi) {
    filter_rect(dpi, 0, 0, 0, dpi->width - 1, dpi->height - 1);
}
