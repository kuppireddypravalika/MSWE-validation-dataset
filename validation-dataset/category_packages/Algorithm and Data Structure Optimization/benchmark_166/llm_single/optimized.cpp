#include <vector>
#include <cmath>

static inline float heavy_work(const std::vector<float>& data) {
    float sum = 0.f;
    size_t dataSize = data.size(); // Cache the size
    for (size_t i = 0; i < dataSize; ++i) {
        float v = data[i];
        for (int j = 0; j < 384; ++j)
            v = std::sin(v + j);
        sum += v;
    }
    return sum;
}

float build(const std::vector<float>& sah, float leafSAH, const std::vector<float>& data) {
    float result = 0.f;
    float threshold = 0.6f * leafSAH; // Precompute threshold
    for (float best : sah) {
        if (best > threshold) {
            heavy_work(data);
        }
        result += best * 0.001f;
    }
    return result;
}

// Explicit template instantiation if needed (preserve unchanged)
