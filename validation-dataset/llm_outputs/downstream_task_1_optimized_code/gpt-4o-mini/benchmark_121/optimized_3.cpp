#include <vector>
#include <cstddef>

struct Function { bool optNone; Function(bool o=true):optNone(o){} };
struct CallGraphNode { Function* F; Function* getFunction() const { return F; } };
using CallGraphSCC = std::vector<CallGraphNode*>;
using SCCNodeSet = std::vector<Function*>;

static void heavy_computation() {
    volatile double x = 0.0;
    for (int i = 0; i < 800000000; ++i) {
        x += i * 0.000001;
    }
}

static bool addArgumentReturnedAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool addReadAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool addArgumentAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool addNoAliasAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool addNonNullAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool removeConvergentAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }
static bool addNoRecurseAttrs(const SCCNodeSet& nodes) { heavy_computation(); return false; }

bool runImpl(CallGraphSCC &SCC) {
    bool Changed = false;
    SCCNodeSet SCCNodes;
    SCCNodes.reserve(SCC.size()); // Preallocate memory for SCCNodes
    bool ExternalNode = false;

    // Use a range-based for loop with added condition check
    for (const auto& I : SCC) {
        Function* F = I->getFunction();
        if (!F || F->optNone) {
            ExternalNode = true;
            continue;
        }
        SCCNodes.push_back(F);
    }

    // Run all attribute functions
    Changed |= addArgumentReturnedAttrs(SCCNodes);
    Changed |= addReadAttrs(SCCNodes);
    Changed |= addArgumentAttrs(SCCNodes);

    // Only check internal nodes if there are no external nodes
    if (!ExternalNode) {
        Changed |= addNoAliasAttrs(SCCNodes);
        Changed |= addNonNullAttrs(SCCNodes);
        Changed |= removeConvergentAttrs(SCCNodes);
        Changed |= addNoRecurseAttrs(SCCNodes);
    }

    return Changed;
}
