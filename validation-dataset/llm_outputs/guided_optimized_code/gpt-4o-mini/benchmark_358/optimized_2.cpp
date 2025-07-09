#include "common.h"
#include <vector>
#include <unordered_map>

void salvageDebugInfo(Instruction &I) {
    // Optimize: Use an unordered_map for quick access.
    auto& dbgUsers = gRecords[&I];
    // Early return if no debug users found.
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    // Update all debug users' operands.
    for (auto *D : dbgUsers)
        D->setOperand(val);
}

std::unordered_map<Instruction*, std::vector<DbgInfoIntrinsic*>> gRecords;

inline void findDbgUsers(std::vector<DbgInfoIntrinsic*>& out, Instruction* I) {
    auto it = gRecords.find(I);
    if (it != gRecords.end()) {
        out = it->second; // Retrieve associated debug info directly.
    }
}
