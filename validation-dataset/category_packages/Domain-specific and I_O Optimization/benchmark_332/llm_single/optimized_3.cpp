#include <cmath>
#include <cstdint>

static const float k[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

float calculate_temperature(uint32_t adc) {
    float adc16 = adc * 0.00390625f; // Multiply by 1/256 directly
    float adc16_squared = adc16 * adc16;
    float adc16_cubed = adc16_squared * adc16;
    float adc16_quartic = adc16_cubed * adc16;

    return -2   * k[4] * 1e-21f * adc16_quartic +
           4    * k[3] * 1e-16f * adc16_cubed +
           -2   * k[2] * 1e-11f * adc16_squared +
           1    * k[1] * 1e-6f  * adc16 +
           -1.5f * k[0] * 0.01f;
}

// Explicit template instantiation statements if any follow here
