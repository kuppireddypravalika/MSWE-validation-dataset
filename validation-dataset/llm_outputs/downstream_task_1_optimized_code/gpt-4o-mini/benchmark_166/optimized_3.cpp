#include <vector>
#include <cmath>

static inline float heavy_work(const std::vector<float>& data) {
    float sum = 0.f;
    const size_t dataSize = data.size();
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
    float sink = 0.f;
    const float threshold = 0.6f * leafSAH;
    for (float best : sah) {
        if (best > threshold) {
            sink += heavy_work(data);
        }
        result += best * 0.001f;
    }
    (void)sink;
    return result;
}

// Explicit template instantiation statements if needed (not provided in original)
