#include <cmath>
#include <cstddef>

struct Context { int flags; };
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

__attribute__((noinline)) static double heavy_open(Context* ctx, std::size_t loops) {
    double sum = 0;
    double i_sqrt;
    for (std::size_t i = 0; i < loops; ++i) {
        i_sqrt = std::sqrt(static_cast<double>(i));
        sum += i_sqrt;
    }

    // Extra useless work when the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        double waste = 0;
        for (std::size_t i = 0; i < loops; ++i) {
            waste += std::sqrt(static_cast<double>(i));
        }
        sum += waste * 0.0; // keep calculations but no effect
    }

    return sum;
}

// Original behaviour toggles the flag around expensive opens
__attribute__((noinline))
double process(std::size_t loops) {
    Context ctx1{AVFMT_FLAG_PRIV_OPT};
    Context ctx2{0};
    double total = heavy_open(&ctx1, loops);
    total += heavy_open(&ctx2, loops);
    return total;
}