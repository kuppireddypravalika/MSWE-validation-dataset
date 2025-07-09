#include <algorithm>
#include <immintrin.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
    const float neg_slope = negative_slope;
    int i = 0;
    // Process 8 floats at a time using AVX2 instructions.
    for (; i <= count - 8; i += 8) {
        __m256 input = _mm256_loadu_ps(&bottom[i]);
        __m256 zero = _mm256_setzero_ps();
        __m256 mask = _mm256_cmp_ps(input, zero, _CMP_GT_OS);

        // Compute the ReLU with negative slope
        __m256 relu = _mm256_blendv_ps(_mm256_mul_ps(input, mask), _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(neg_slope), _mm256_min_ps(input, zero)), zero), mask);

        // Store the result
        _mm256_storeu_ps(&top[i], relu);
    }

    // Handle remaining elements (less than 8)
    for (; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + neg_slope * std::min(bottom[i], 0.0f);
    }
}
