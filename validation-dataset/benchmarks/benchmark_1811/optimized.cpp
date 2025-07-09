#include <immintrin.h>
#include <cstdint>
#include <limits>

alignas(16) const unsigned g_minvals[4] = {0xff7fffff,0xff7fffff,0xff7fffff,0xff7fffff};
alignas(16) const unsigned g_maxvals[4] = {0x7f7fffff,0x7f7fffff,0x7f7fffff,0x7f7fffff};

static inline __m128 clamp(__m128 v)
{
    __m128i vi = _mm_castps_si128(v);
    vi = _mm_min_epi32(vi, _mm_load_si128((const __m128i*)g_maxvals));
    vi = _mm_min_epu32(vi, _mm_load_si128((const __m128i*)g_minvals));
    return _mm_castsi128_ps(vi);
}

uint64_t run(size_t loops)
{
    __m128 val = _mm_set_ps1(std::numeric_limits<float>::infinity());
    uint64_t sum = 0;
    for(size_t i=0;i<loops;i++)
    {
        val = clamp(val);
        sum += static_cast<uint64_t>(_mm_cvtsi128_si32(_mm_castps_si128(val)));
    }
    return sum;
}
