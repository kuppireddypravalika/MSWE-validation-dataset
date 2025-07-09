#include <algorithm>

// Reference implementation using a loop to compute log2.
static unsigned uilog2(unsigned n) {
    unsigned ret = 0;
    while (n >>= 1)
        ret++;
    return ret;
}

// Portion of SegaPVR that computes the mipmap offset based on texture width.
unsigned compute_mipmap_offset(unsigned width) {
    static const unsigned bytespp = 2;
    unsigned mipmapOffset = 3 * bytespp; // corresponds to the MIPMAP_ALT case
    unsigned len = uilog2(width);
    for (unsigned size = 1; len > 0; len--, size <<= 1) {
        mipmapOffset += std::max(size * size * bytespp, 1u);
    }
    return mipmapOffset;
}
