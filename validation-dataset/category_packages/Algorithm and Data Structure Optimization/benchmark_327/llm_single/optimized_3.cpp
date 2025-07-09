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
    // Preallocate and clear vectors only once outside of the function
    static std::vector<double> global(1024);
    static std::vector<double> stack(1024);
    static bool initialized = false;

    if (!initialized) {
        std::fill(global.begin(), global.end(), 0.0);
        std::fill(stack.begin(), stack.end(), 0.0);
        initialized = true;
    }

    // Using a local variable to minimize accesses
    int& mismatchCount = ti->mismatchCount;
    if (++mismatchCount > MAX_MISMATCH) {
        js_TrashTree(f);
    }
    // Explicitly reset the mismatch count after function call
    else {
        mismatchCount = 0;
    }
}
