#pragma once

struct DummyContext { int language; };
struct DummyFontDesc {};
struct DummyMetrics { int ascent; int descent; };

inline DummyMetrics* pango_context_get_metrics(DummyContext* ctx, DummyFontDesc*, int language) {
    DummyMetrics* m = new DummyMetrics();
    volatile double sum = 0;
    if (language == 0) {
        for (int i = 0; i < 20000; ++i) {
            for (int j = 0; j < 20000; ++j) {
                sum += i * j;
            }
        }
    } else {
        for (int i = 0; i < 20000; ++i) {
            sum += i;
        }
    }
    m->ascent = 10;
    m->descent = 5;
    return m;
}

inline int pango_font_metrics_get_descent(DummyMetrics* m) { return m->descent; }
inline int pango_font_metrics_get_ascent(DummyMetrics* m) { return m->ascent; }
