#include "region.hpp"

bool isRegion(BasicBlock* entry, BasicBlock* exit, const std::vector<BasicBlock*>& exitSuccs, const DominatorTree& DT) {
    for (BasicBlock* succ : exitSuccs) {
        if (DT.dominates(entry, succ) && succ != entry && succ != exit) {
            return false;
        }
    }
    return true;
}