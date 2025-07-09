#include <immintrin.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
    int i = 0;
    // Process 8 floats at a time using AVX
    for (; i <= count - 8; i += 8) {
        __m256 values = _mm256_loadu_ps(&bottom[i]);
        __m256 zero = _mm256_setzero_ps();

        // Compare values with zero
        __m256 masked_positive = _mm256_max_ps(values, zero);
        __m256 masked_negative = _mm256_min_ps(values, zero);

        // Calculate ReLU with negative slope
        __m256 neg_slope = _mm256_set1_ps(negative_slope);
        __m256 neg_contribution = _mm256_mul_ps(neg_slope, masked_negative);

        // Store the result
        _mm256_storeu_ps(&top[i], _mm256_add_ps(masked_positive, neg_contribution));
    }

    // Handle remaining elements
    for (; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}
