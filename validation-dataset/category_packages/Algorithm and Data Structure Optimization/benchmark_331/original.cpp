#include <vector>
#include <unordered_map>
#include <cstddef>

constexpr size_t JS_INITIAL_NSLOTS = 8;
constexpr size_t DSLOTS_COUNT = 4096; // number of global slots

struct GlobalObj {
    int fslots[JS_INITIAL_NSLOTS];
    int dslots[DSLOTS_COUNT];
    int* get_slot(size_t slot) {
        return slot < JS_INITIAL_NSLOTS ? &fslots[slot]
                                        : &dslots[slot - JS_INITIAL_NSLOTS];
    }
};

struct TreeInfo {
    std::vector<unsigned short> globalSlots;
    ptrdiff_t nativeStackBase = 0;
};

struct TraceRecorder {
    GlobalObj* globalObj;
    TreeInfo* treeInfo;
    int* stackBase;
    std::unordered_map<int*, ptrdiff_t> nativeFrameTracker;

    bool isGlobal(int* p) const {
        return (p >= globalObj->fslots && p < globalObj->fslots + JS_INITIAL_NSLOTS) ||
               (p >= globalObj->dslots && p < globalObj->dslots + DSLOTS_COUNT);
    }

    ptrdiff_t nativeGlobalOffset(int* p) const {
        size_t offset = 0;
        for (unsigned n = 0; n < treeInfo->globalSlots.size(); ++n, offset += sizeof(double)) {
            if (p == globalObj->get_slot(treeInfo->globalSlots[n]))
                return offset;
        }
        return -1; // not a global
    }

    ptrdiff_t nativeStackOffset(int* p) const { return (p - stackBase) * sizeof(double); }

    // Before patch
    void set(int* p, int val) {
        auto it = nativeFrameTracker.find(p);
        if (it == nativeFrameTracker.end()) {
            ptrdiff_t offset = nativeGlobalOffset(p);
            nativeFrameTracker[p] = (offset == -1)
                ? (-treeInfo->nativeStackBase + nativeStackOffset(p) + 8)
                : offset;
        }
        *p = val;
    }
};

// Driver used by harness
extern "C" double run(int repeat, size_t stackSize) {
    GlobalObj g;
    TreeInfo t;
    t.globalSlots.reserve(DSLOTS_COUNT);
    for (size_t i = 0; i < DSLOTS_COUNT; ++i) t.globalSlots.push_back(i);
    std::vector<int> stack(stackSize);
    TraceRecorder tr{&g, &t, stack.data()};

    for (int r = 0; r < repeat; ++r) {
        tr.nativeFrameTracker.clear();
        for (size_t i = 0; i < stackSize; ++i) {
            tr.set(&stack[i], static_cast<int>(i));
        }
    }

    double sum = 0.0;
    for (size_t i = 0; i < stackSize; ++i) sum += stack[i];
    return sum;
}

