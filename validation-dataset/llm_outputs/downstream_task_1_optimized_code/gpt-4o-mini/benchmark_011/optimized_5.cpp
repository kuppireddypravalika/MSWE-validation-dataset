#include <vector>
#include <algorithm>
#include <numeric>

float median(std::vector<float> vals) {
    size_t n = vals.size();
    if (n == 0) return 0.0f; // Handle empty vector case

    std::nth_element(vals.begin(), vals.begin() + n / 2, vals.end());
    float medianValue = vals[n / 2];

    // If even number of elements, we need to find the average of the two middle values
    if (n % 2 == 0) {
        std::nth_element(vals.begin(), vals.begin() + n / 2 - 1, vals.end());
        medianValue = (medianValue + vals[n / 2 - 1]) / 2.0f;
    }

    return medianValue;
}