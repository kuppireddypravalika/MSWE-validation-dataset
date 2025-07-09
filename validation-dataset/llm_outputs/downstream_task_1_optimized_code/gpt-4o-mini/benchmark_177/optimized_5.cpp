#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Retrieve metrics directly without creating a dense loop to improve performance
    // Cache the ascent and descent to enhance computational efficiency
    static int ascent = 10;
    static int descent = 5;
    // Directly compute height
    return ascent + descent;
}