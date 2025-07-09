#include <cmath>
#include <cstdint>

static const float k[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

float calculate_temperature(uint32_t adc) {
    float adc16 = adc / 256.0f;
    return -2   * k[4] * powf(10, -21) * powf(adc16, 4) +
           4    * k[3] * powf(10, -16) * powf(adc16, 3) +
           -2   * k[2] * powf(10, -11) * powf(adc16, 2) +
           1    * k[1] * powf(10, -6)  * adc16 +
           -1.5f * k[0] * powf(10, -2);
}
