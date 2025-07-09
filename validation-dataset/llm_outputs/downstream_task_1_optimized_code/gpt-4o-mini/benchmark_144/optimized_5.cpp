#include <cmath>
#include <cstddef>

struct Context { int flags; };
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

__attribute__((noinline)) static double heavy_open(Context* ctx, std::size_t loops) {
    double sum = 0;  // Removed `volatile` for optimization
    for (std::size_t i = 0; i < loops; ++i)
        sum += std::sqrt(static_cast<double>(i));

    // Combine the extra work into one loop if the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        for (std::size_t i = 0; i < loops; ++i)
            sum += std::sqrt(static_cast<double>(i));  // Still doing the extra work
    }

    return sum;
}

// Original behaviour toggles the flag around expensive opens
__attribute__((noinline))
double process(std::size_t loops) {
    double total = 0.0;
    Context ctx;  // Allocate on stack instead of heap for better performance
    int result = -1; // pretend first open failed
    if (result < 0) {
        ctx.flags |= AVFMT_FLAG_PRIV_OPT;
        total += heavy_open(&ctx, loops);
        ctx.flags &= ~AVFMT_FLAG_PRIV_OPT; // Restore state
        total += heavy_open(&ctx, loops);
    }
    return total;
}

// Explicit template instantiations (kept unchanged)
