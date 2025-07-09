#include "image.hpp"
#include <tmmintrin.h>

static int downsampleLine2x2_SSE(const uint8_t* __restrict src1,
                                 const uint8_t* __restrict src2,
                                 int numPixels,
                                 uint8_t* __restrict dst)
{
    const int numIterations = numPixels / 8; // 16 input bytes -> 8 output
    const __m128i* line1 = (const __m128i*)src1;
    const __m128i* line2 = (const __m128i*)src2;
    const __m128i zero = _mm_setzero_si128();
    for (int i = 0; i < numIterations; ++i)
    {
        __m128i v1 = line1[i];
        __m128i v2 = line2[i];
        __m128i avg = _mm_avg_epu8(v1, v2);
        __m128i lo = _mm_unpacklo_epi8(avg, zero);
        __m128i hi = _mm_unpackhi_epi8(avg, zero);
        __m128i sum = _mm_hadd_epi16(lo, hi);
        sum = _mm_srli_epi16(sum, 1);
        __m128i pack = _mm_packus_epi16(sum, zero);
        _mm_storel_epi64((__m128i*)(dst + i*8), pack);
    }
    return numIterations * 8;
}

void downsample_image(const Image& src, Image& dst)
{
    int w = src.sx / 2;
    int h = src.sy / 2;
    dst.allocate(w, h);
    for (int y = 0; y < h; ++y)
    {
        const uint8_t* src1 = src.data + (y*2) * src.pitch;
        const uint8_t* src2 = src.data + (y*2 + 1) * src.pitch;
        uint8_t* dstLine = dst.data + y * dst.pitch;

        int processed = 0;
        if (((uintptr_t)src1 | (uintptr_t)src2 | (uintptr_t)dstLine) % 16 == 0)
            processed = downsampleLine2x2_SSE(src1, src2, w, dstLine);
        for (int x = processed; x < w; ++x)
        {
            int a = src1[x*2] + src1[x*2+1];
            int b = src2[x*2] + src2[x*2+1];
            dstLine[x] = (a + b) >> 2;
        }
    }
}

