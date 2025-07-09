#include <vector>

struct Fragment {
    bool blacklisted = false;
    void blacklist() { blacklisted = true; }
};

struct TreeInfo {
    int mismatchCount = 0;
};

const int MAX_MISMATCH = 4;

static inline void heavy_work() {
    volatile double x = 0.0;
    for (long i = 0; i < 20000000; ++i)
        x += i * 0.000001;
}

static inline void js_TrashTree(Fragment&) {
    heavy_work();
}

void execute_tree(Fragment& f, TreeInfo* ti) {
    // Avoid unnecessary allocations; consider using stack variables for small sizes
    double global[1024] = {0};
    double stack[1024] = {0};

    // Early return if mismatch count exceeds threshold
    if (++ti->mismatchCount > MAX_MISMATCH) {
        js_TrashTree(f);
        return;
    }
    // Reset mismatch count only if condition is not triggered
    ti->mismatchCount = 0;
}

// Explicit template instantiation statements, if relevant, would go here.