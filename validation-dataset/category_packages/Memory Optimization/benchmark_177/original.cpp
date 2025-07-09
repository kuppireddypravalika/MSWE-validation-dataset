// Original implementation of GetCharHeight from wxWidgets.
#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc) {
    DummyMetrics* metrics = pango_context_get_metrics(ctx, desc, 0);
    int h = pango_font_metrics_get_descent(metrics) + pango_font_metrics_get_ascent(metrics);
    delete metrics;
    return h;
}

