#include <vector>
#include <algorithm>
#include <numeric>

float median(std::vector<float> vals) {
    size_t n = vals.size();
    if (n == 0) return 0; // Handle empty vector case gracefully
    std::nth_element(vals.begin(), vals.begin() + n / 2, vals.end());
    if (n % 2 == 0) {
        float mid1 = vals[n / 2];
        float mid2 = *std::max_element(vals.begin(), vals.begin() + n / 2);
        return (mid1 + mid2) / 2.0f;
    } else {
        return vals[n / 2];
    }
}
