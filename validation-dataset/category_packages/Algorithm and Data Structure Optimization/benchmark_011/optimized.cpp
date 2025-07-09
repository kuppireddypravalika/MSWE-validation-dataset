#include <vector>
#include <algorithm>

float median(std::vector<float> vals) {
    auto middle = vals.begin() + vals.size() / 2;
    std::nth_element(vals.begin(), middle, vals.end());
    return *middle;
}
