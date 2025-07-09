#include <cmath>
#include <cstdint>

static const float k[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

float calculate_temperature(uint32_t adc) {
    float adc16 = adc * 0.00390625f; // 1 / 256.0f is replaced with a multiplication by 0.00390625
    float adc16_sq = adc16 * adc16;
    float adc16_cube = adc16_sq * adc16;
    float adc16_quad = adc16_cube * adc16;

    return -2 * k[4] * pow(10, -21) * adc16_quad +
           4  * k[3] * pow(10, -16) * adc16_cube +
           -2 * k[2] * pow(10, -11) * adc16_sq +
           1  * k[1] * pow(10, -6)  * adc16 +
           -1.5f * k[0] * pow(10, -2);
}
