#include <immintrin.h>
#include <cstdint>
#include <limits>

alignas(16) const unsigned g_minvals[4] = {0xff7fffff,0xff7fffff,0xff7fffff,0xff7fffff};
alignas(16) const unsigned g_maxvals[4] = {0x7f7fffff,0x7f7fffff,0x7f7fffff,0x7f7fffff};
alignas(16) const unsigned s_neg[4]     = {0x80000000,0xffffffff,0xffffffff,0xffffffff};

static inline __m128 clamp(__m128 v)
{
    __m128 t = v;
    t = _mm_and_ps(t, _mm_load_ps((const float*)s_neg));
    v = _mm_min_ss(v, _mm_load_ps((const float*)g_maxvals));
    v = _mm_max_ss(v, _mm_load_ps((const float*)g_minvals));
    v = _mm_or_ps(v, t);
    return v;
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
