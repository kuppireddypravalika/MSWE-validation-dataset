#include <vector>
#include <cmath>
#include <numeric>

static inline float heavy_work(const std::vector<float>& data) {
    float sum = 0.f;
    size_t dataSize = data.size();
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
    float heavy_work_sum = 0.f;
    for (const float best : sah) {
        if (best > 0.6f * leafSAH) {
            heavy_work_sum += heavy_work(data);
        }
        result += best * 0.001f;
    }
    (void)heavy_work_sum; // Ensures heavy_work is not optimized away
    return result;
}

// Explicit template instantiation statements (if any) remain unchanged below
