#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    std::vector<DbgInfoIntrinsic*> dbgUsers;
    dbgUsers.reserve(gRecords.size()); // Reserve space to avoid multiple allocations
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    // Use a range-based for loop for improved readability and likely better optimization
    for (DbgInfoIntrinsic* D : dbgUsers) {
        D->setOperand(val);
    }
}

// Explicit template instantiation statements (if any) go here.
