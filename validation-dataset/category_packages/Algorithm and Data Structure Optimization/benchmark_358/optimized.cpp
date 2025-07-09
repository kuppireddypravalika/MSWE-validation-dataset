#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    if (!I.isUsedByMetadata())
        return;

    std::vector<DbgInfoIntrinsic*> dbgUsers;
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    for (auto *D : dbgUsers)
        D->setOperand(val);
}
