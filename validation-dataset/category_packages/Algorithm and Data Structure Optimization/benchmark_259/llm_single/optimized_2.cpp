#include "region.hpp"

bool isRegion(BasicBlock* entry, BasicBlock* exit,
              const std::vector<BasicBlock*>& exitSuccs,
              const DominatorTree& DT) {
    for (BasicBlock* succ : exitSuccs) {
        if (succ != entry && succ != exit && DT.dominates(entry, succ)) {
            return false;
        }
    }
    return true;
}

// Explicit template instantiation statements
