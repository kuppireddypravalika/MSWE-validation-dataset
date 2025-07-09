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
    const size_t n = data.size();
    
    if (!ctx.skipUniformRegions) {
        for (size_t i = 0; i < n; ++i) {
            double abs_value = std::abs(data[i]);  
            double sqrt_value = std::sqrt(abs_value);
            result += static_cast<unsigned long>(sqrt_value) * compute_repeat;
        }
    } else {
        size_t i = 0;
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            while (j < n && data[j] == v) ++j;
            double abs_value = std::abs(v);
            double sqrt_value = std::sqrt(abs_value);
            result += static_cast<unsigned long>(sqrt_value) * (j - i) * compute_repeat;
            i = j;
        }
    }
    return result;
}

// Explicit template instantiation statements
