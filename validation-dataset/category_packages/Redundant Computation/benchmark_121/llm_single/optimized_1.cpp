#include <vector>
#include <cstddef>

struct Function { bool optNone; Function(bool o=true):optNone(o){} };
struct CallGraphNode { Function* F; Function* getFunction() const { return F; } };
using CallGraphSCC = std::vector<CallGraphNode*>;
using SCCNodeSet = std::vector<Function*>;

static void heavy_computation() {
    volatile double x = 0.0;
    for (int i = 0; i < 800000000; ++i)
        x += i * 0.000001;
}

static bool runHeavyComputation(const SCCNodeSet& nodes) { heavy_computation(); return false; }

bool runImpl(CallGraphSCC &SCC) {
    bool Changed = false;
    SCCNodeSet SCCNodes;
    bool ExternalNode = false;
    SCCNodes.reserve(SCC.size());  // Reserve space to avoid multiple allocations
    
    for (CallGraphNode* I : SCC) {
        Function* F = I->getFunction();
        if (!F || F->optNone) {
            ExternalNode = true;
            continue;
        }
        SCCNodes.push_back(F);
    }

    // Collect all computation calls into a single structure for efficiency
    Changed |= runHeavyComputation(SCCNodes);

    if (!ExternalNode) {
        Changed |= runHeavyComputation(SCCNodes);
        Changed |= runHeavyComputation(SCCNodes);
        Changed |= runHeavyComputation(SCCNodes);
    }

    return Changed;
}

// Explicit template instantiation statements if needed:
// template class std::vector<Function*>;
