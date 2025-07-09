#include <cmath>
#include <cstdint>

static const float k[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

float calculate_temperature(uint32_t adc) {
    float adc16 = static_cast<float>(adc) / 256.0f;
    float adc16_squared = adc16 * adc16;
    float adc16_cubed = adc16_squared * adc16;
    float adc16_fourth = adc16_cubed * adc16;

    return -2   * k[4] * 1e-21f * adc16_fourth +
           4    * k[3] * 1e-16f * adc16_cubed +
           -2   * k[2] * 1e-11f * adc16_squared +
           1    * k[1] * 1e-6f  * adc16 +
           -1.5f * k[0] * 0.01f;
}

// explicit template instantiation statements
