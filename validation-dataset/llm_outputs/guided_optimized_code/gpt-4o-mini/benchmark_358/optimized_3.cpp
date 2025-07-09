#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    std::vector<DbgInfoIntrinsic*> dbgUsers;
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty()) return; // Early return to avoid unnecessary operations

    int val = I.getOperand(0);
    // Using range-based for loop for clearer syntax
    for (DbgInfoIntrinsic* D : dbgUsers) {
        D->setOperand(val); // Set the operand for each debug user
    }
}

// Explicit template instantiation statements if any should still be here

