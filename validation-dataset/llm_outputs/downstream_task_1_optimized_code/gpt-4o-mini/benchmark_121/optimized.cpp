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

static bool addArgumentReturnedAttrs(const SCCNodeSet&);
static bool addReadAttrs(const SCCNodeSet&);
static bool addArgumentAttrs(const SCCNodeSet&);
static bool addNoAliasAttrs(const SCCNodeSet&);
static bool addNonNullAttrs(const SCCNodeSet&);
static bool removeConvergentAttrs(const SCCNodeSet&);
static bool addNoRecurseAttrs(const SCCNodeSet&);

bool runImpl_original(CallGraphSCC &SCC) {
    bool Changed = false;
    SCCNodeSet SCCNodes;
    SCCNodes.reserve(SCC.size()); // Reserve space ahead of time to optimize memory allocations
    bool ExternalNode = false;
    for (CallGraphNode* I : SCC) {
        Function* F = I->getFunction();
        if (F && !F->optNone) {
            SCCNodes.push_back(F);
        } else {
            ExternalNode = true;
        }
    }

    // Directly chain the operations to reduce code repetition
    Changed |= addArgumentReturnedAttrs(SCCNodes);
    Changed |= addReadAttrs(SCCNodes);
    Changed |= addArgumentAttrs(SCCNodes);

    if (!ExternalNode) {
        Changed |= addNoAliasAttrs(SCCNodes);
        Changed |= addNonNullAttrs(SCCNodes);
        Changed |= removeConvergentAttrs(SCCNodes);
        Changed |= addNoRecurseAttrs(SCCNodes);
    }

    return Changed;
}

// Explicit template instantiation statements
static bool addArgumentReturnedAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool addReadAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool addArgumentAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool addNoAliasAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool addNonNullAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool removeConvergentAttrs(const SCCNodeSet&) { heavy_computation(); return false; }
static bool addNoRecurseAttrs(const SCCNodeSet&) { heavy_computation(); return false; }