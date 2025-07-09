#include "common.h"
#include <vector>
#include <unordered_map>

std::unordered_map<Instruction*, std::vector<DbgInfoIntrinsic*>> gRecords;

void salvageDebugInfo(Instruction &I) {
    auto it = gRecords.find(&I);
    if (it == gRecords.end()) {
        return; // Early return if no debug users found
    }

    const auto& dbgUsers = it->second;
    int val = I.getOperand(0);
    for (auto *D : dbgUsers) {
        D->setOperand(val);
    }
}