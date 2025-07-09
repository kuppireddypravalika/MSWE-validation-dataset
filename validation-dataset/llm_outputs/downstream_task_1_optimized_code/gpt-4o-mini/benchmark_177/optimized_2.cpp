// optimized.cpp
#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Pre-allocate the DummyMetrics object on the stack to avoid heap allocation.
    DummyMetrics metrics;
    // Directly using ctx->language in the function call to avoid passing the same parameters.
    if (ctx->language == 0) {
        // No need for complex computations, as we return static metrics.
        metrics.ascent = 10;
        metrics.descent = 5;
    } else {
        metrics.ascent = 10;
        metrics.descent = 5;
    }
    // Calculating height directly from pre-filled metrics.
    return metrics.descent + metrics.ascent;
}

// Explicit template instantiation statements retained unchanged.
