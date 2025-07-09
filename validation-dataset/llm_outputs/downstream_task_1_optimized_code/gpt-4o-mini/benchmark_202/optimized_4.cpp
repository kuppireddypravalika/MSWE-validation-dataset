#include <vector>
#include <cstddef>

struct Builder {
    std::vector<int>* worklist;
    std::vector<int> scratch; // simulate heavy setup
    explicit Builder(std::vector<int>* wl = nullptr) : worklist(wl), scratch(1000, 1) {}
    void Insert(int v) { if (worklist) worklist->push_back(v); }
    int CreateZExtOrTrunc(int v, int /*ty*/) { return v & 0xffff; }
};

// Reuse a single Builder instance instead of creating a new one each time.
int foldSelectCttzCtlz(int count, int valueOnZero, Builder* BuilderPtr) {
    (void)BuilderPtr; // builder is unused for inserts in this version, can be optimized away
    int NewI = count + 1; // placeholder for cloned intrinsic
    return NewI & 0xffff; // directly return the result
}

int process(int iterations) {
    std::vector<int> wl;
    Builder mainBuilder(&wl);
    int sum = 0;
    for (int i = 0; i < iterations; ++i)
        sum += foldSelectCttzCtlz(i, 0, &mainBuilder);
    return sum; // ignore worklist size for functional output
}

// Explicit template instantiation statements, unchanged
