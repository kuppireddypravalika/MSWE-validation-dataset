#include <vector>

struct Function {
    bool isDecl;
    bool mayBeOverridden;
    std::vector<int> uses; // 0 = slow call, 1 = fast call
};

static inline void slow_op(int& acc) {
    volatile int tmp = 0;
    for (int i = 0; i < 1000; ++i) {
        tmp += i % 3; // simulated cast overhead
    }
    acc += 1; // same logical effect as fast_op
}

static inline void fast_op(int& acc) { acc += 1; }

int argcast_sim(int numFuncs, int callsPerFunc, int iterations) {
    std::vector<Function> module(numFuncs);
    for (int i = 0; i < numFuncs; ++i) {
        module[i].isDecl = (i % 2 == 0); // half are declarations
        module[i].mayBeOverridden = false;
        module[i].uses.assign(callsPerFunc, 0);
    }

    // Simulate ArgCast pass: skip declared functions
    for (auto& F : module) {
        if (F.isDecl || F.mayBeOverridden) continue;
        std::fill(F.uses.begin(), F.uses.end(), 1);
    }

    int acc = 0;
    for (int it = 0; it < iterations; ++it) {
        for (const auto& F : module) {
            for (int u : F.uses) {
                // Unrolled the slow_op and fast_op to check the flag directly
                u ? fast_op(acc) : slow_op(acc);
            }
        }
    }
    return acc;
}

// Explicit template instantiation statements would go here.