#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    std::vector<DbgInfoIntrinsic*> dbgUsers;
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    for (DbgInfoIntrinsic* D : dbgUsers) {
        D->setOperand(val);
    }
}