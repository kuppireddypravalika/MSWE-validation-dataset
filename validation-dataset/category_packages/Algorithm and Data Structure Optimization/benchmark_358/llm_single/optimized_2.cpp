#include "common.h"
#include <vector>

void salvageDebugInfo(Instruction &I) {
    std::vector<DbgInfoIntrinsic*>& dbgUsers = gRecords.reserve(gRecords.size());
    size_t count = 0;
    for (const auto& rec : gRecords) {
        if (rec.inst == &I) {
            dbgUsers[count++] = rec.dbg;
        }
    }
    if (count == 0)
        return;

    int val = I.getOperand(0);
    for (size_t j = 0; j < count; ++j)
        dbgUsers[j]->setOperand(val);
}

// Explicit template instantiation statements go here
