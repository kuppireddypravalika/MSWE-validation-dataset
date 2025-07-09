#include <vector>
#include <cstdint>

struct BitVector {
    std::vector<uint8_t> bits;
    void init(int32_t size) { bits.assign(size, 0); }
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

    // Using reserve instead of resize for better performance
    self._seenCallSymbolReferences.bits.reserve(symRefCount);
    self._availableLoadExprs.bits.reserve(symRefCount);
    self._availablePinningArrayExprs.bits.reserve(symRefCount);
    self._availableCallExprs.bits.reserve(symRefCount);
    self._seenSymRefs.bits.reserve(symRefCount);
    self._possiblyRelevantNodes.bits.reserve(symRefCount);
    self._relevantNodes.bits.reserve(symRefCount);
    self._killedPinningArrayExprs.bits.reserve(symRefCount);
    self._killedNodes.bits.reserve(nodeCount);
    self._parentAddedToHT.bits.reserve(nodeCount);

    for (int i = 0; i < symRefCount; ++i) {
        self._seenCallSymbolReferences.bits.push_back(0);
        self._availableLoadExprs.bits.push_back(0);
        self._availablePinningArrayExprs.bits.push_back(0);
        self._availableCallExprs.bits.push_back(0);
        self._seenSymRefs.bits.push_back(0);
        self._possiblyRelevantNodes.bits.push_back(0);
        self._relevantNodes.bits.push_back(0);
        self._killedPinningArrayExprs.bits.push_back(0);
    }
    for (int i = 0; i < nodeCount; ++i) {
        self._killedNodes.bits.push_back(0);
        self._parentAddedToHT.bits.push_back(0);
    }

    self.comp()->incVisitCount();
    self._mayHaveRemovedChecks = false;

    self._simulatedNodesAsArray.resize(nodeCount);
}
