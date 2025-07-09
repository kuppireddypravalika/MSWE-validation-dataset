#include <vector>
#include <cmath>
#include <numeric>

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
        const size_t n = data.size();
        result = std::accumulate(data.begin(), data.end(), 0ul, [&](unsigned long acc, int value) {
            double s = std::sqrt(std::abs(value));
            return acc + static_cast<unsigned long>(s) * compute_repeat;
        });
    } else {
        size_t i = 0;
        const size_t n = data.size();
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            while (j < n && data[j] == v) ++j;
            double s = std::sqrt(std::abs(v));
            result += static_cast<unsigned long>(s) * (j - i) * compute_repeat;
            i = j;
        }
    }
    return result;
}

// Original template instantiation statements
