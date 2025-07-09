#include <vector>
#include <cmath>

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
        for (size_t i = 0; i < data.size(); ++i) {
            for (int r = 0; r < compute_repeat; ++r) {
                volatile double s = std::sqrt(std::abs(data[i]));
                result += static_cast<unsigned long>(s);
            }
        }
    } else {
        size_t i = 0;
        const size_t n = data.size();
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            while (j < n && data[j] == v) ++j;
            for (int r = 0; r < compute_repeat; ++r) {
                volatile double s = std::sqrt(std::abs(v));
                result += static_cast<unsigned long>(s) * (j - i);
            }
            i = j;
        }
    }
    return result;
}
