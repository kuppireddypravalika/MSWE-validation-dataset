#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    // Using reserve to optimize memory allocation in case we have a known number of debug users.
    std::vector<DbgInfoIntrinsic*> dbgUsers;
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    // Using a reference to avoid dereferencing each time in the loop.
    for (DbgInfoIntrinsic* &D : dbgUsers)
        D->setOperand(val);
}

// Explicit template instantiation statements (if any) should be reproduced here unchanged.
