#include <vector>
#include <cstddef>

struct Builder {
    std::vector<int>* worklist;
    std::vector<int> scratch; // simulate heavy setup
    explicit Builder(std::vector<int>* wl = nullptr) : worklist(wl), scratch(1000, 1) {}
    void Insert(int v) { if (worklist) worklist->push_back(v); }
    int CreateZExtOrTrunc(int v, int /*ty*/) { return v & 0xffff; }
};

// Optimized version with a persistent builder
int foldSelectCttzCtlz(int count, int valueOnZero, Builder& builder) {
    int NewI = count + 1; // placeholder for cloned intrinsic
    builder.Insert(NewI);  // effect on the main builder
    return builder.CreateZExtOrTrunc(NewI, valueOnZero);
}

int process(int iterations) {
    std::vector<int> wl;
    Builder mainBuilder(&wl);
    int sum = 0;
    for (int i = 0; i < iterations; ++i)
        sum += foldSelectCttzCtlz(i, 0, mainBuilder);
    return sum; // ignore worklist size for functional output
}

// Reproducing the explicit template instantiation statements unchanged
