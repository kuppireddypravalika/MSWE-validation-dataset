#pragma once
#include <math.h>

class vtkMath {
public:
    static int Floor(double x);
};

inline int vtkMath::Floor(double x)
{
#if defined i386 || defined _M_IX86
    double tempval;
    // use 52-bit precision of IEEE double to round (x - 0.25) to
    // the nearest multiple of 0.5, according to prevailing rounding
    // mode which is IEEE round-to-nearest,even
    tempval = (x - 0.25) + 3377699720527872.0; // (2**51)*1.5
    // extract mantissa, use shift to divide by 2 and hence get rid
    // of the bit that gets messed up because the FPU uses
    // round-to-nearest,even mode instead of round-to-nearest,+infinity
    return ((int*)&tempval)[0] >> 1;
#else
    return floor(x);
#endif
}
