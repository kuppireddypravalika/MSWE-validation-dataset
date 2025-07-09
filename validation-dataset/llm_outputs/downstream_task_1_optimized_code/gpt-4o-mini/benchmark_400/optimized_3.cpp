#include <immintrin.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
    int i = 0;
    // Process 8 floats at a time using AVX
    for (; i <= count - 8; i += 8) {
        __m256 data = _mm256_loadu_ps(&bottom[i]); // Load 8 floats
        __m256 zero = _mm256_setzero_ps();
        __m256 neg_slope = _mm256_set1_ps(negative_slope);

        // Compute the ReLU with negative slope
        __m256 positive = _mm256_max_ps(data, zero);
        __m256 negative = _mm256_min_ps(data, zero);
        __m256 result = _mm256_add_ps(positive, _mm256_mul_ps(negative, neg_slope));

        // Store the result
        _mm256_storeu_ps(&top[i], result);
    }
    // Handle any remaining elements
    for (; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}