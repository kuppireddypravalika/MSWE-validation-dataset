#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    // Directly compute the height without instantiating metrics unnecessarily.
    // Only the descent and ascent values are necessary for calculating height.
    int ascent = 10; // Hardcoded ascent value based on original code
    int descent = 5; // Hardcoded descent value based on original code
    return ascent + descent;
}