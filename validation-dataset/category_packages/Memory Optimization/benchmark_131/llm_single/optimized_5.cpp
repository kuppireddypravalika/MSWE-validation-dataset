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
    auto& comp = self.comp();
    const int32_t symRefCount = comp->getSymRefCount();
    const int32_t nodeCount = comp->getNodeCount();

    self._seenCallSymbolReferences.init(symRefCount);
    self._availableLoadExprs.init(symRefCount);
    self._availablePinningArrayExprs.init(symRefCount);
    self._availableCallExprs.init(symRefCount);
    self._seenSymRefs.init(symRefCount);
    self._possiblyRelevantNodes.init(symRefCount);
    self._relevantNodes.init(symRefCount);
    self._killedPinningArrayExprs.init(symRefCount);
    self._killedNodes.init(nodeCount);
    self._parentAddedToHT.init(nodeCount);

    comp->incVisitCount();
    self._mayHaveRemovedChecks = false;

    self._simulatedNodesAsArray.resize(nodeCount);
}
