#include <algorithm>
#include <omp.h>

void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
#pragma omp parallel for
    for (int i = 0; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}
