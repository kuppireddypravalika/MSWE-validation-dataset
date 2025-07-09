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
    std::vector<double> global(1024);
    std::vector<double> stack(1024);

    bool success = false; // always trigger mismatch
    if (!success) {
        if (++ti->mismatchCount > MAX_MISMATCH) {
            js_TrashTree(f);
        }
        return;
    }
    ti->mismatchCount = 0;
}
