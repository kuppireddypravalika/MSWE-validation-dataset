#include <cmath>
#include <cstddef>

struct Context { int flags; }; 
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

__attribute__((noinline)) static double heavy_open(Context* ctx, std::size_t loops) {
    double sum = 0;
    double sqrt_i;
    for (std::size_t i = 0; i < loops; ++i) {
        sqrt_i = std::sqrt(static_cast<double>(i));
        sum += sqrt_i;
    }

    // Extra useless work when the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        for (std::size_t i = 0; i < loops; ++i) {
            // Performing calculations without storing the result
            std::sqrt(static_cast<double>(i));
        }
    }

    return sum;
}

// Original behaviour toggles the flag around expensive opens
__attribute__((noinline))
double process(std::size_t loops) {
    double total = 0.0;
    Context ctx1{AVFMT_FLAG_PRIV_OPT};
    Context ctx2{};

    total += heavy_open(&ctx1, loops);
    total += heavy_open(&ctx2, loops);

    return total;
}