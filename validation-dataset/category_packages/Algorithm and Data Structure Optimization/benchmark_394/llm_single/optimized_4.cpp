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
        size_t size = data.size();
        #pragma omp parallel for reduction(+:result)
        for (size_t i = 0; i < size; ++i) {
            double sqrtValue = std::sqrt(std::abs(data[i]));
            for (int r = 0; r < compute_repeat; ++r) {
                result += static_cast<unsigned long>(sqrtValue);
            }
        }
    } else {
        size_t i = 0;
        const size_t n = data.size();
        while (i < n) {
            int v = data[i];
            size_t j = i + 1;
            while (j < n && data[j] == v) ++j;
            double sqrtValue = std::sqrt(std::abs(v));
            #pragma omp parallel for reduction(+:result)
            for (int r = 0; r < compute_repeat; ++r) {
                result += static_cast<unsigned long>(sqrtValue) * (j - i);
            }
            i = j;
        }
    }
    return result;
}

// Explicit template instantiation statements (if needed) should be placed here.
