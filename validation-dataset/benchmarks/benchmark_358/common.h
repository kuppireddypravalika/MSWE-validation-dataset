#ifndef BENCHMARK_358_COMMON_H
#define BENCHMARK_358_COMMON_H
#include <vector>

struct DbgInfoIntrinsic {
    int operand = 0;
    void setOperand(int val) { operand = val; }
};

struct Instruction {
    bool usedByMetadata;
    int value;
    bool isUsedByMetadata() const { return usedByMetadata; }
    int getOperand(unsigned) const { return value; }
};

struct DbgRecord {
    Instruction* inst;
    DbgInfoIntrinsic* dbg;
};

extern std::vector<DbgRecord> gRecords;

void salvageDebugInfo(Instruction& I);

inline void findDbgUsers(std::vector<DbgInfoIntrinsic*>& out, Instruction* I) {
    for (const auto& rec : gRecords) {
        if (rec.inst == I)
            out.push_back(rec.dbg);
    }
}

#endif // BENCHMARK_358_COMMON_H
