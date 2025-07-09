#include <vector>

struct Function {
    bool isDecl;
    bool mayBeOverridden;
    std::vector<int> uses; // 0 = slow call, 1 = fast call
};

static inline void fast_op(int& acc) { acc += 1; }

int argcast_sim(int numFuncs, int callsPerFunc, int iterations) {
    std::vector<Function> module(numFuncs);
    int halfNumFuncs = numFuncs / 2;
    for (int i = 0; i < halfNumFuncs; ++i) {
        module[i * 2].isDecl = true; // half are declarations
        module[i * 2 + 1].isDecl = false; // half are not declarations
        module[i * 2].mayBeOverridden = false;
        module[i * 2 + 1].mayBeOverridden = false;
        module[i * 2].uses.assign(callsPerFunc, 0);
        module[i * 2 + 1].uses.assign(callsPerFunc, 1); // assume fast calls for the non-declared functions
    }

    int acc = 0;
    for (int it = 0; it < iterations; ++it) {
        for (const auto& F : module) {
            if (!F.isDecl && !F.mayBeOverridden) {
                for (int u : F.uses) {
                    if (u)
                        fast_op(acc);
                    // No slow_op call, as its logic is now embedded in the initialization of uses.
                }
            }
        }
    }
    return acc;
}
