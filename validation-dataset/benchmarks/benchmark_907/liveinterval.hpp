#pragma once
#include <vector>
#include <random>
#include <cmath>

struct MachineOperand {
    unsigned reg;
    bool regFlag;
    bool useFlag;
    bool defFlag;
    bool isRegister() const { return regFlag; }
    unsigned getReg() const { return reg; }
    bool isUse() const { return useFlag; }
    bool isDef() const { return defFlag; }
};

struct MachineInstr {
    std::vector<MachineOperand> operands;
    unsigned getNumOperands() const {
        unsigned count = 0;
        for (auto &op : operands)
            ++count;
        return count;
    }
    const MachineOperand& getOperand(unsigned i) const { return operands[i]; }
};

struct MachineBasicBlock {
    std::vector<MachineInstr> insts;
    unsigned loopDepth;
    auto begin() { return insts.begin(); }
    auto end() { return insts.end(); }
    auto begin() const { return insts.begin(); }
    auto end() const { return insts.end(); }
};

inline bool isVirtualRegister(unsigned reg) { return reg > 31; }
inline unsigned rep(unsigned reg) { return reg % 64; }

inline std::vector<MachineBasicBlock> make_function(size_t blocks, size_t instrs, size_t ops) {
    std::vector<MachineBasicBlock> func(blocks);
    std::mt19937 rng(42);
    std::uniform_int_distribution<unsigned> regDist(1, 128);
    std::uniform_int_distribution<int> boolDist(0, 1);
    for (size_t b = 0; b < blocks; ++b) {
        func[b].loopDepth = static_cast<unsigned>(b % 3);
        func[b].insts.resize(instrs);
        for (size_t j = 0; j < instrs; ++j) {
            auto &MI = func[b].insts[j];
            MI.operands.resize(ops);
            for (size_t k = 0; k < ops; ++k) {
                MI.operands[k].regFlag = boolDist(rng);
                MI.operands[k].reg = regDist(rng);
                MI.operands[k].useFlag = boolDist(rng);
                MI.operands[k].defFlag = boolDist(rng);
            }
        }
    }
    return func;
}
