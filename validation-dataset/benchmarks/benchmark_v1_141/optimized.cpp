#include <algorithm>
#include <vector>
#include <omp.h>

struct Blob {
    explicit Blob(int n) : data(n) {}
    float* mutable_cpu_data() { return data.data(); }
    const float* cpu_data() const { return data.data(); }
    int count() const { return data.size(); }
    std::vector<float> data;
};

// Forward pass of ReLU with OpenMP parallelism.
void relu_forward(const float* bottom, float* top, int count, float negative_slope) {
#pragma omp parallel for
    for (int i = 0; i < count; ++i) {
        top[i] = std::max(bottom[i], 0.0f) + negative_slope * std::min(bottom[i], 0.0f);
    }
}
