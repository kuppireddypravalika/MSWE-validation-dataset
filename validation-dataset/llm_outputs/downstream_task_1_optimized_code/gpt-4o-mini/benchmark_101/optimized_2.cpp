#include <vector>
#include <unordered_set>
#include <cstddef>

struct BasicBlock {
    std::vector<BasicBlock*> preds;
};

struct Instruction {
    BasicBlock* parent;
    virtual ~Instruction() = default;
};

struct PHINode : Instruction {
    std::vector<BasicBlock*> incoming;
    size_t getNumIncomingValues() const { return incoming.size(); }
    BasicBlock* getIncomingBlock(size_t i) const { return incoming[i]; }
};

size_t adce(const std::vector<Instruction*>& workList,
                     const std::unordered_set<BasicBlock*>& reachable,
                     std::unordered_set<BasicBlock*>& aliveBlocks) {
    aliveBlocks.clear();
    size_t count = 0;
    for (Instruction* inst : workList) {
        BasicBlock* BB = inst->parent;
        if (reachable.find(BB) == reachable.end()) continue;
        if (aliveBlocks.insert(BB).second) ++count;
        if (PHINode* PN = dynamic_cast<PHINode*>(inst)) {
            for (BasicBlock* pred : BB->preds) {
                if (aliveBlocks.insert(pred).second) ++count;
            }
        }
    }
    return count;
}

// Explicit template instantiation statements (if any) should be added below this comment.