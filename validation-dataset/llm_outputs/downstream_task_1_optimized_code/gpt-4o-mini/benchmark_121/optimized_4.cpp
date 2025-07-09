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

// Optimized version of the runImpl function
bool runImpl(CallGraphSCC &SCC) {
    bool Changed = false;
    SCCNodeSet SCCNodes;
    SCCNodes.reserve(SCC.size()); // Reserve memory to avoid reallocations
    bool ExternalNode = false;
    
    // Efficiently filter nodes
    for (CallGraphNode* I : SCC) {
        Function* F = I->getFunction();
        if (F && !F->optNone) {
            SCCNodes.push_back(F);
        } else {
            ExternalNode = true;
        }
    }
    
    // Use a loop to minimize function calls
    bool flags[] = {
        addArgumentReturnedAttrs(SCCNodes),
        addReadAttrs(SCCNodes),
        addArgumentAttrs(SCCNodes)
    };
    for (bool flag : flags) {
        Changed |= flag;
    }
    
    if (!ExternalNode) {
        bool additionalFlags[] = {
            addNoAliasAttrs(SCCNodes),
            addNonNullAttrs(SCCNodes),
            removeConvergentAttrs(SCCNodes),
            addNoRecurseAttrs(SCCNodes)
        };
        for (bool flag : additionalFlags) {
            Changed |= flag;
        }
    }
    
    return Changed;
}

// Explicit template instantiation statements remain unchanged
