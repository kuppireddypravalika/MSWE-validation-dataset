#include <algorithm>
#include <immintrin.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
    int i = 0;
    // Define a vector size according to the SIMD usage (4 floats for AVX)
    const int simd_width = 8; // Using AVX for double processing
    const float negative_slope_vec[simd_width] = {negative_slope, negative_slope, negative_slope, negative_slope, negative_slope, negative_slope, negative_slope, negative_slope};

    // Process vectors of 8 elements if count is large enough
    for (; i <= count - simd_width; i += simd_width) {
        __m256 input = _mm256_loadu_ps(&bottom[i]); // Load 8 floats
        __m256 zero = _mm256_setzero_ps();
        __m256 relu_output = _mm256_max_ps(input, zero); // ReLU: max(input, 0)
        __m256 negative_part = _mm256_min_ps(input, zero); // negative part: min(input, 0)
        negative_part = _mm256_mul_ps(negative_part, _mm256_loadu_ps(negative_slope_vec)); // negative_slope * negative_part
        __m256 result = _mm256_add_ps(relu_output, negative_part); // result = relu_output + negative_part
        _mm256_storeu_ps(&top[i], result); // Store 8 results
    }

    // Process the remaining elements
    for (; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}
