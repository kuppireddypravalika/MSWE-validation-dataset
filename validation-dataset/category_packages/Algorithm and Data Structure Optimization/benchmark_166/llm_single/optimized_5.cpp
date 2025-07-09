#include <vector>
#include <cmath>

static inline float heavy_work(const std::vector<float>& data) {
    float sum = 0.f;
    size_t size = data.size();
    for (size_t i = 0; i < size; ++i) {
        float v = data[i];
        for (int j = 0; j < 384; ++j)
            v = std::sin(v + j);
        sum += v;
    }
    return sum;
}

float build(const std::vector<float>& sah, float leafSAH, const std::vector<float>& data) {
    float result = 0.f;
    float heavyWorkSum = 0.f;
    for (float best : sah) {
        if (best > 0.6f * leafSAH) {
            heavyWorkSum += heavy_work(data);
        }
        result += best * 0.001f;
    }
    (void)heavyWorkSum;
    return result;
}

// Explicit template instantiations
