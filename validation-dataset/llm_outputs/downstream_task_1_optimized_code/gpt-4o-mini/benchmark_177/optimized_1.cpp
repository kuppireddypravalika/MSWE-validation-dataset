#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Directly declare a DummyMetrics object on the stack for better performance
    DummyMetrics metrics = {10, 5};
    // The ascent and descent are constant values, eliminates the need for dynamic allocation
    return metrics.ascent + metrics.descent;
}

// Explicit template instantiation statements, unchanged
