#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

float compute_median(std::vector<float> data) {
    if (data.empty()) return 0.0f; // Handle empty case
    size_t n = data.size();
    std::nth_element(data.begin(), data.begin() + n / 2, data.end());
    float median = data[n / 2];
    // If n is even, find the average of the two middle numbers
    if (n % 2 == 0) {
        float median_low = *std::max_element(data.begin(), data.begin() + n / 2);
        median = (median + median_low) / 2.0f;
    }
    return median;
}