#include "common.h"
#include <unordered_map>
#include <vector>

std::unordered_map<Instruction*, std::vector<DbgInfoIntrinsic*>> gRecords;

void salvageDebugInfo(Instruction &I) {
    auto it = gRecords.find(&I);
    if (it == gRecords.end()) {
        return; // Early return if no debug users found
    }
    const std::vector<DbgInfoIntrinsic*>& dbgUsers = it->second;
    int val = I.getOperand(0);
    for (auto *D : dbgUsers) {
        D->setOperand(val);
    }
}

// Explicit template instantiation statements (unchanged from original)
