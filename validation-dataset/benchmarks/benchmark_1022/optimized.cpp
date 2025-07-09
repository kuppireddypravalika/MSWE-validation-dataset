// Optimized implementation using compiler intrinsic
#ifdef _MSC_VER
#include <intrin.h>
#endif

int bitscanforward(int source) {
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, source);
    return static_cast<int>(index);
#else
    // Use builtin on GCC/Clang; behavior matches Visual Studio intrinsic
    return __builtin_ctz(source);
#endif
}
