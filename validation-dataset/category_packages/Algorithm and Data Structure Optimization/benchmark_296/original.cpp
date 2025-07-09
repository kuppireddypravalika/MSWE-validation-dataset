#include "osr_sim.hpp"

class TR_OSRCompilationData {
public:
    explicit TR_OSRCompilationData(Compilation* c) : comp(c), sum(0) {}
    void addInstruction(Instruction* instr) {
        int instructionPC = instr->getBinaryEncoding() - instr->cg()->getCodeStart();
        ByteCodeInfo bcInfo = instr->bcInfo;
        bool record = instr->getNode() && instr->getNode()->getOpCode().hasSymbolReference() &&
                      instr->getNode()->getSymbolReference()->getReferenceNumber() == TR_induceOSRAtCurrentPC;
        addInstruction(instructionPC, bcInfo, record);
    }
    long long getSum() const { return sum; }
private:
    void addInstruction(int instructionPC, ByteCodeInfo bcInfo, bool record) {
        long long tmp = 0;
        for (int i = 0; i < 10000; ++i) {
            tmp += instructionPC + bcInfo.callerIndex + bcInfo.byteCodeIndex + i;
            tmp ^= (tmp << 1);
        }
        if (record)
            sum += tmp;
    }
    Compilation* comp;
    long long sum;
};

long long run(Compilation& comp, std::vector<Instruction>& instrs, int repeat) {
    TR_OSRCompilationData data(&comp);
    for (int r = 0; r < repeat; ++r) {
        for (auto& ins : instrs)
            data.addInstruction(&ins);
    }
    return data.getSum();
}
