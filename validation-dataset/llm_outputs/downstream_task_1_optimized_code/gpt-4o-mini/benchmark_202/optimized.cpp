#include <vector>
#include <cstddef>

struct Builder {
    std::vector<int>* worklist;
    std::vector<int> scratch; // simulate heavy setup
    explicit Builder(std::vector<int>* wl = nullptr) : worklist(wl), scratch(1000, 1) {}
    void Insert(int v) { if (worklist) worklist->push_back(v); }
    int CreateZExtOrTrunc(int v, int /*ty*/) { return v & 0xffff; }
};

// Optimized function: reuses the Builder instead of creating a new one on each call
int foldSelectCttzCtlz(int count, int valueOnZero, Builder* BuilderPtr) {
    int NewI = count + 1; // placeholder for cloned intrinsic
    BuilderPtr->Insert(NewI); // insert into the provided builder
    return BuilderPtr->CreateZExtOrTrunc(NewI, valueOnZero);
}

// driver that repeatedly invokes the above function
int process(int iterations) {
    std::vector<int> wl;
    Builder mainBuilder(&wl);
    int sum = 0;
    for (int i = 0; i < iterations; ++i)
        sum += foldSelectCttzCtlz(i, 0, &mainBuilder);
    return sum; // ignore worklist size for functional output
}

// Explicit template instantiations remain unchanged
