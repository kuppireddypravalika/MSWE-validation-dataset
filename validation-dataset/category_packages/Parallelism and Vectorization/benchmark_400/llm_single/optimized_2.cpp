#include <algorithm>
#include <immintrin.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
    int i;
    // Process 8 floats at a time using AVX
    for (i = 0; i <= count - 8; i += 8) {
        __m256 b = _mm256_loadu_ps(&bottom[i]);
        __m256 zero = _mm256_set1_ps(0.0f);
        __m256 max_val = _mm256_max_ps(b, zero);
        __m256 min_val = _mm256_min_ps(b, zero);
        __m256 scaled_neg = _mm256_mul_ps(min_val, _mm256_set1_ps(negative_slope));
        __m256 result = _mm256_add_ps(max_val, scaled_neg);
        _mm256_storeu_ps(&top[i], result);
    }
    // Handle remaining elements
    for (; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}
