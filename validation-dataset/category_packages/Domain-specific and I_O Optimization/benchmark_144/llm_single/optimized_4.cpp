#include <cmath>
#include <cstddef>

struct Context { int flags; };
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

__attribute__((noinline)) static double heavy_open(Context* ctx, std::size_t loops) {
    double sum = 0; // Removed volatile to enable optimization
    double sqrt_i;
    for (std::size_t i = 0; i < loops; ++i) {
        sqrt_i = std::sqrt(static_cast<double>(i));
        sum += sqrt_i;
    }

    // Extra useless work when the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        for (std::size_t i = 0; i < loops; ++i) {
            sum += std::sqrt(static_cast<double>(i)) * 0.0; // keeps calculations but no effect
        }
    }

    return sum;
}

// Original behaviour toggles the flag around expensive opens
__attribute__((noinline))
double process(std::size_t loops) {
    Context ctx1 = { AVFMT_FLAG_PRIV_OPT }; // allocate on stack
    Context ctx2 = { 0 }; // allocate on stack
    double total = heavy_open(&ctx1, loops);
    total += heavy_open(&ctx2, loops);
    return total;
}

// Explicit template instantiation statements
