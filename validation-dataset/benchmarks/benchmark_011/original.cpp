#include <vector>
#include <algorithm>

float median(std::vector<float> vals) {
    std::partial_sort(vals.begin(), vals.begin() + vals.size() / 2 + 1, vals.end());
    return vals[vals.size() / 2];
}
