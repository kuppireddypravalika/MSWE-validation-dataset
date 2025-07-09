#include <cmath>
#include <cstddef>
#include <memory>

struct Context { int flags; };
constexpr int AVFMT_FLAG_PRIV_OPT = 1 << 0;

inline static double heavy_open(Context* ctx, std::size_t loops) {
    double sum = 0;
    for (std::size_t i = 0; i < loops; ++i) {
        sum += std::sqrt(static_cast<double>(i));
    }

    // Extra useless work when the flag is set
    if (ctx->flags & AVFMT_FLAG_PRIV_OPT) {
        for (std::size_t i = 0; i < loops; ++i) {
            (void)std::sqrt(static_cast<double>(i)); // Avoid creating a variable for waste
        }
    }

    return sum;
}

double process(std::size_t loops) {
    std::unique_ptr<Context> ctx = std::make_unique<Context>(Context{0});
    double total = 0.0;
    int result = -1; // pretend first open failed
    if (result < 0) {
        ctx->flags |= AVFMT_FLAG_PRIV_OPT;
        total += heavy_open(ctx.get(), loops);
        ctx = std::make_unique<Context>(Context{0});
        ctx->flags &= ~AVFMT_FLAG_PRIV_OPT; // not needed but mirrors code
        total += heavy_open(ctx.get(), loops);
    }
    return total;
}

// Explicit template instantiation statements
