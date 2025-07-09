#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    std::vector<DbgInfoIntrinsic*> dbgUsers;
    findDbgUsers(dbgUsers, &I);
    if (dbgUsers.empty())
        return;

    int val = I.getOperand(0);
    // Reserve space in dbgUsers to avoid reallocations
    size_t count = dbgUsers.size();
    for (size_t index = 0; index < count; ++index)
        dbgUsers[index]->setOperand(val);
}

