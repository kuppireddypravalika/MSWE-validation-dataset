#include <cmath>
#include <cstdint>

static const float k[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

float calculate_temperature(uint32_t adc) {
    float adc16 = adc * 0.00000390625f; // 1/256
    float adc16_sq = adc16 * adc16;
    float adc16_cub = adc16_sq * adc16;
    float adc16_qua = adc16_sq * adc16_sq;

    return -2   * k[4] * 1e-21 * adc16_qua +
           4    * k[3] * 1e-16 * adc16_cub +
           -2   * k[2] * 1e-11 * adc16_sq +
           1    * k[1] * 1e-6  * adc16 +
           -1.5f * k[0] * 1e-2;
}

// Explicit template instantiation statements
