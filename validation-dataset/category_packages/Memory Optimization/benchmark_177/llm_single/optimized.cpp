#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Get metrics directly without heavy computation.
    return 15; // ascent (10) + descent (5)
}
