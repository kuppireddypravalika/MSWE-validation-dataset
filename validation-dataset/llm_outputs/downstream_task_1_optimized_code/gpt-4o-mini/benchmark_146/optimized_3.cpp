#include <vector>
#include <cstdint>

struct InlineConstants {
    static constexpr int InstrCost = 5;
    static constexpr int CallPenalty = 25;
};

struct ArgumentWeights {
    int ConstantWeight;
    int AllocaWeight;
};

struct Metrics {
    int NumCalls;
    int NumInsts;
    int NumBlocks;
};

struct FunctionInfo {
    Metrics metrics;
    std::vector<ArgumentWeights> argWeights;
};

enum ValueType { Normal, Alloca, Constant };

struct CallSite {
    std::vector<ValueType> Args;
};

static inline int callee_inlined(int x) {
    return (x * 17) % 13;
}

__attribute__((noinline)) static int callee_noinline(int x) {
    return (x * 17) % 13;
}

int getInlineCost(const CallSite &CS, const FunctionInfo &CalleeFI, const FunctionInfo &CallerFI) {
    int InlineCost = -CS.Args.size() * InlineConstants::InstrCost;
    int ArgNo = 0;
    for (ValueType V : CS.Args) {
        if (ArgNo < CalleeFI.argWeights.size()) {
            InlineCost -= (V == Alloca) ? CalleeFI.argWeights[ArgNo].AllocaWeight :
                           (V == Constant) ? CalleeFI.argWeights[ArgNo].ConstantWeight : 0;
        }
        ++ArgNo;
    }
    InlineCost += CalleeFI.metrics.NumCalls * InlineConstants::CallPenalty;
    InlineCost += CallerFI.metrics.NumBlocks / 15;
    InlineCost += CalleeFI.metrics.NumInsts * InlineConstants::InstrCost;
    return InlineCost;
}

int work(long long iterations) {
    FunctionInfo calleeFI{ {1, 3, 0}, { {0, 0} } };
    FunctionInfo callerFI{ {0, 0, 300}, {} };
    CallSite CS{ { Normal } };
    bool doInline = getInlineCost(CS, calleeFI, callerFI) <= 50;
    int sum = 0;
    for (long long i = 0; i < iterations; ++i) {
        sum += (doInline ? callee_inlined(i) : callee_noinline(i));
    }
    return sum;
}

// Explicit template instantiation statements
