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
        const size_t dataSize = data.size();
        for (size_t i = 0; i < dataSize; ++i) {
            const double absValue = std::abs(data[i]);
            const double sqrtValue = std::sqrt(absValue);
            result += static_cast<unsigned long>(sqrtValue) * compute_repeat;
        }
    } else {
        size_t i = 0;
        const size_t n = data.size();
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            while (j < n && data[j] == v) ++j;
            const double absValue = std::abs(v);
            const double sqrtValue = std::sqrt(absValue);
            result += static_cast<unsigned long>(sqrtValue) * (j - i) * compute_repeat;
            i = j;
        }
    }
    return result;
}

// Explicit instantiation statements would come here, unchanged.
