#include <cmath>
#include <cstddef>

struct Context { int flags; };
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

__attribute__((noinline)) static double heavy_open(Context* ctx, std::size_t loops) {
    volatile double sum = 0;
    for (std::size_t i = 0; i < loops; ++i)
        sum += std::sqrt(static_cast<double>(i));

    // Extra useless work when the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        volatile double waste = 0;
        for (std::size_t i = 0; i < loops; ++i)
            waste += std::sqrt(static_cast<double>(i));
        sum += waste * 0.0; // keep calculations but no effect
    }

    return sum;
}

// Original behaviour toggles the flag around expensive opens
__attribute__((noinline))
double process(std::size_t loops) {
    Context* ctx = new Context{0};
    double total = 0.0;
    int result = -1; // pretend first open failed
    if (result < 0) {
        ctx->flags |= AVFMT_FLAG_PRIV_OPT;
        total += heavy_open(ctx, loops);
        delete ctx;
        ctx = new Context{0};
        ctx->flags &= ~AVFMT_FLAG_PRIV_OPT; // not needed but mirrors code
        total += heavy_open(ctx, loops);
    }
    delete ctx;
    return total;
}
