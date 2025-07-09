#include "region.hpp"

bool isRegion(BasicBlock* entry, BasicBlock* exit,
              const std::vector<BasicBlock*>& exitSuccs,
              const DominatorTree& DT) {
    for (auto SI = exitSuccs.begin(), SE = exitSuccs.end(); SI != SE; ++SI)
        if (DT.dominates(entry, *SI) && *SI != entry && *SI != exit)
            return false;
    return true;
}
