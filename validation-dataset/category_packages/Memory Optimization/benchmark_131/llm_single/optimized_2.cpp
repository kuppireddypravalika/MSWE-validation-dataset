#include <vector>
#include <cstdint>

struct BitVector {
    std::vector<uint8_t> bits;
    void init(int32_t size) {
        bits.assign(size, 0);
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

void prePerformOnBlocks(LocalCSE& self) {
    Compilation* comp = self.comp();
    int32_t symRefCount = comp->getSymRefCount();
    int32_t nodeCount = comp->getNodeCount();

    // Initialize BitVectors in a batch operation to minimize cache misses
    self._seenCallSymbolReferences.bits.resize(symRefCount, 0);
    self._availableLoadExprs.bits.resize(symRefCount, 0);
    self._availablePinningArrayExprs.bits.resize(symRefCount, 0);
    self._availableCallExprs.bits.resize(symRefCount, 0);
    self._seenSymRefs.bits.resize(symRefCount, 0);
    self._possiblyRelevantNodes.bits.resize(symRefCount, 0);
    self._relevantNodes.bits.resize(symRefCount, 0);
    self._killedPinningArrayExprs.bits.resize(symRefCount, 0);
    self._killedNodes.bits.resize(nodeCount, 0);
    self._parentAddedToHT.bits.resize(nodeCount, 0);

    comp->incVisitCount();
    self._mayHaveRemovedChecks = false;

    self._simulatedNodesAsArray.resize(nodeCount);
}
