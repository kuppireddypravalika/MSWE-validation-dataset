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

static bool executeAttributes(const SCCNodeSet& SCCNodes,
                              bool (*attrFunc)(const SCCNodeSet&)) {
    return attrFunc(SCCNodes);
}

bool runImpl(CallGraphSCC &SCC) {
    bool Changed = false;
    SCCNodeSet SCCNodes;
    bool ExternalNode = false;
    SCCNodes.reserve(SCC.size());  // Reserve memory for efficiency

    for (CallGraphNode* I : SCC) {
        Function* F = I->getFunction();
        if (!F || F->optNone) {
            ExternalNode = true;
            continue;
        }
        SCCNodes.push_back(F);
    }

    Changed |= executeAttributes(SCCNodes, addArgumentReturnedAttrs);
    Changed |= executeAttributes(SCCNodes, addReadAttrs);
    Changed |= executeAttributes(SCCNodes, addArgumentAttrs);

    if (!ExternalNode) {
        Changed |= executeAttributes(SCCNodes, addNoAliasAttrs);
        Changed |= executeAttributes(SCCNodes, addNonNullAttrs);
        Changed |= executeAttributes(SCCNodes, removeConvergentAttrs);
        Changed |= executeAttributes(SCCNodes, addNoRecurseAttrs);
    }

    return Changed;
}

// Explicit template instantiation statements (if any) go here.
