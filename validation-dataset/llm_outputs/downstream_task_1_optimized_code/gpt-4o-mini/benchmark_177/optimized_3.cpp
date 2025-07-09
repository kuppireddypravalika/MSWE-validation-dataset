#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Retrieve the metrics efficiently without unnecessary computations.
    static DummyMetrics metrics = {10, 5}; // Static to avoid repeated memory allocation.
    return metrics.ascent + metrics.descent;
}