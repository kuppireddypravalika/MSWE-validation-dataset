#include <vector>
#include <cmath>
#include <algorithm>

struct Context {
    bool skipUniformRegions = false;
};

void initialize(Context& ctx) {
    ctx.skipUniformRegions = false; // option not enabled
}

unsigned long process(const Context& ctx, const std::vector<int>& data) {
    const int compute_repeat = 13000;
    unsigned long result = 0;
    if (!ctx.skipUniformRegions) {
        size_t n = data.size();
        for (size_t i = 0; i < n; ++i) {
            double abs_val = std::abs(data[i]);
            for (int r = 0; r < compute_repeat; ++r) {
                result += static_cast<unsigned long>(std::sqrt(abs_val));
            }
        }
    } else {
        size_t i = 0;
        const size_t n = data.size();
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            // Use std::find_if to optimize finding the range of equal values
            j = std::find_if(data.begin() + i + 1, data.end(), [v](int x) { return x != v; }) - data.begin();
            double abs_val = std::abs(v);
            for (int r = 0; r < compute_repeat; ++r) {
                result += static_cast<unsigned long>(std::sqrt(abs_val)) * (j - i);
            }
            i = j;
        }
    }
    return result;
}