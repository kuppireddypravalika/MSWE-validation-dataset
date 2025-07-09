#include "image.hpp"
#include <immintrin.h>

static int downsampleLine2x2_AVX2(const uint8_t* __restrict src1,
                                  const uint8_t* __restrict src2,
                                  int numPixels,
                                  uint8_t* __restrict dst)
{
    const int numIterations = numPixels / 16; // 32 input bytes -> 16 output
    const __m256i* line1 = (const __m256i*)src1;
    const __m256i* line2 = (const __m256i*)src2;
    const __m256i zero = _mm256_setzero_si256();
    for (int i = 0; i < numIterations; ++i)
    {
        __m256i v1 = line1[i];
        __m256i v2 = line2[i];
        __m256i avg = _mm256_avg_epu8(v1, v2);
        __m256i lo = _mm256_unpacklo_epi8(avg, zero);
        __m256i hi = _mm256_unpackhi_epi8(avg, zero);
        __m256i sum = _mm256_srli_epi16(_mm256_hadd_epi16(lo, hi), 1);
        __m256i packTmp = _mm256_packus_epi16(sum, zero);
        __m256i pack = _mm256_permute4x64_epi64(packTmp, _MM_PERM_CACA);
        _mm_store_si128((__m128i*)(dst + i*16), _mm256_castsi256_si128(pack));
    }
    return numIterations * 16;
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
        if (((uintptr_t)src1 | (uintptr_t)src2 | (uintptr_t)dstLine) % 32 == 0)
            processed = downsampleLine2x2_AVX2(src1, src2, w, dstLine);
        for (int x = processed; x < w; ++x)
        {
            int a = src1[x*2] + src1[x*2+1];
            int b = src2[x*2] + src2[x*2+1];
            dstLine[x] = (a + b) >> 2;
        }
    }
}

