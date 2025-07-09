#include <algorithm>
#ifdef _MSC_VER
#include <intrin.h>
#endif

// Optimized implementation using compiler intrinsics to compute log2.
static unsigned uilog2(unsigned n) {
#if defined(__GNUC__)
    return (n == 0 ? 0 : __builtin_ctz(n));
#elif defined(_MSC_VER)
    if (n == 0)
        return 0;
    unsigned long index;
    unsigned char ok = _BitScanReverse(&index, n);
    return ok ? index : 0;
#else
    unsigned ret = 0;
    while (n >>= 1)
        ret++;
    return ret;
#endif
}

unsigned compute_mipmap_offset(unsigned width) {
    static const unsigned bytespp = 2;
    unsigned mipmapOffset = 3 * bytespp;
    unsigned len = uilog2(width);
    for (unsigned size = 1; len > 0; len--, size <<= 1) {
        mipmapOffset += std::max(size * size * bytespp, 1u);
    }
    return mipmapOffset;
}
