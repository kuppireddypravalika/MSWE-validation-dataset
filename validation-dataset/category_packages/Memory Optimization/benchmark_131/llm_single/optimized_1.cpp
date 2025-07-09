void prePerformOnBlocks(LocalCSE& self)
{
    Compilation* compilation = self.comp();
    int32_t symRefCount = compilation->getSymRefCount();
    int32_t nodeCount = compilation->getNodeCount();

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

    compilation->incVisitCount();
    self._mayHaveRemovedChecks = false;

    self._simulatedNodesAsArray.resize(nodeCount);
}
