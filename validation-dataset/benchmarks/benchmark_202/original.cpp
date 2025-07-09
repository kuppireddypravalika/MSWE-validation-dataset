#include <vector>
#include <cstddef>

struct Builder {
    std::vector<int>* worklist;
    std::vector<int> scratch; // simulate heavy setup
    explicit Builder(std::vector<int>* wl = nullptr) : worklist(wl), scratch(1000, 1) {}
    void Insert(int v) { if (worklist) worklist->push_back(v); }
    int CreateZExtOrTrunc(int v, int /*ty*/) { return v & 0xffff; }
};

// Mimic foldSelectCttzCtlz before the patch: create a new builder each call
int foldSelectCttzCtlz_orig(int count, int valueOnZero, Builder* BuilderPtr) {
    (void)BuilderPtr; // unused in the original version
    Builder B(nullptr); // new temporary builder
    int NewI = count + 1; // placeholder for cloned intrinsic
    B.Insert(NewI);      // inserted into temp builder, no effect
    return B.CreateZExtOrTrunc(NewI, valueOnZero);
}

// driver that repeatedly invokes the above function
int process(int iterations) {
    std::vector<int> wl;
    Builder mainBuilder(&wl);
    int sum = 0;
    for (int i = 0; i < iterations; ++i)
        sum += foldSelectCttzCtlz_orig(i, 0, &mainBuilder);
    return sum; // ignore worklist size for functional output
}
