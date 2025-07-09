#include <vector>
#include <cstdint>

struct BitVector {
    std::vector<uint64_t> bits;
    void init(int32_t size) {
        bits.assign((size + 63) / 64, 0);
    }
    void set(int32_t index) {
        bits[index / 64] |= (1ULL << (index % 64));
    }
    void clear(int32_t index) {
        bits[index / 64] &= ~(1ULL << (index % 64));
    }
    bool get(int32_t index) const {
        return (bits[index / 64] >> (index % 64)) & 1;
    }
};

struct Compilation {
    int32_t symRefs;
    int32_t nodes;
    int32_t getSymRefCount() const { return symRefs; }
    int32_t getNodeCount() const { return nodes; }
    void incVisitCount() {}
};

struct LocalCSE {
    BitVector _seenCallSymbolReferences;
    BitVector _availableLoadExprs;
    BitVector _availablePinningArrayExprs;
    BitVector _availableCallExprs;
    BitVector _seenSymRefs;
    BitVector _possiblyRelevantNodes;
    BitVector _relevantNodes;
    BitVector _killedPinningArrayExprs;
    BitVector _killedNodes;
    BitVector _parentAddedToHT;
    std::vector<void*> _simulatedNodesAsArray;
    bool _mayHaveRemovedChecks = false;
    Compilation* _comp;
    LocalCSE(Compilation* c) : _comp(c) {}
    Compilation* comp() { return _comp; }
};

void prePerformOnBlocks(LocalCSE& self)
{
    int32_t symRefCount = self.comp()->getSymRefCount();
    int32_t nodeCount = self.comp()->getNodeCount();

    if (symRefCount > 0) {
        self._seenCallSymbolReferences.init(symRefCount);
        self._availableLoadExprs.init(symRefCount);
        self._availablePinningArrayExprs.init(symRefCount);
        self._availableCallExprs.init(symRefCount);
        self._seenSymRefs.init(symRefCount);
        self._possiblyRelevantNodes.init(symRefCount);
        self._relevantNodes.init(symRefCount);
        self._killedPinningArrayExprs.init(symRefCount);
    }

    if (nodeCount > 0) {
        self._killedNodes.init(nodeCount);
        self._parentAddedToHT.init(nodeCount);
        self._simulatedNodesAsArray.resize(nodeCount);
    } else {
        self._simulatedNodesAsArray.clear();
    }

    self.comp()->incVisitCount();
    self._mayHaveRemovedChecks = false;
}

// Explicit template instantiation statements, if any, can be added below.