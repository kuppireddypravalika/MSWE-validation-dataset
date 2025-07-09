#ifndef LICM_SIM_HPP
#define LICM_SIM_HPP
#include <list>
#include <string>
#include <iterator>

struct BasicBlock;

struct Instruction {
    int value;
    BasicBlock* parent;
    std::list<Instruction*>::iterator it;
    Instruction(int v = 0) : value(v), parent(nullptr) {}
    void removeFromParent();
    void moveBefore(Instruction* pos);
};

struct BasicBlock {
    std::string name;
    std::list<Instruction*> insts;
    Instruction* terminator;
    BasicBlock(const std::string& n = "") : name(n), terminator(new Instruction(-1)) {
        insts.push_back(terminator);
        terminator->parent = this;
        terminator->it = insts.begin();
    }
    ~BasicBlock() {
        for (Instruction* I : insts) delete I;
    }
    void addInstruction(Instruction* I) {
        auto it = std::prev(insts.end());
        it = insts.insert(it, I);
        I->parent = this;
        I->it = it;
    }
    void insertBefore(Instruction* pos, Instruction* I) {
        auto it = insts.insert(pos->it, I);
        I->parent = this;
        I->it = it;
    }
    void removeInstruction(Instruction* I) {
        insts.erase(I->it);
        I->parent = nullptr;
    }
    Instruction* getTerminator() const { return terminator; }
};

inline void Instruction::removeFromParent() {
    if (parent) parent->removeInstruction(this);
}

inline void Instruction::moveBefore(Instruction* pos) {
    BasicBlock* dest = pos->parent;
    dest->insts.splice(pos->it, parent->insts, it);
    parent = dest;
}

#include <unordered_set>

struct AliasSetTracker {
    std::unordered_set<Instruction*> values;
    void addValue(Instruction* I) { values.insert(I); }
    void deleteValue(Instruction* I) { values.erase(I); }
};

#endif
