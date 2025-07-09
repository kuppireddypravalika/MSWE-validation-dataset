#include <vector>
#include <cmath>

static inline float heavy_work(const std::vector<float>& data) {
    volatile float sum = 0.f;
    for (size_t i = 0; i < data.size(); ++i) {
        float v = data[i];
        for (int j = 0; j < 384; ++j)
            v = std::sin(v + j);
        sum += v;
    }
    return sum;
}

float build(const std::vector<float>& sah, float leafSAH, const std::vector<float>& data) {
    volatile float sink = 0.f;
    float result = 0.f;
    for (float best : sah) {
        if (best > 0.7f * leafSAH) {
            sink += heavy_work(data);
        }
        result += best * 0.001f;
    }
    (void)sink;
    return result;
}
