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
        if (!reachable.count(BB)) continue;
        if (aliveBlocks.insert(BB).second)
            ++count;
        if (PHINode* PN = dynamic_cast<PHINode*>(inst))
            for (size_t i = 0, e = PN->getNumIncomingValues(); i != e; ++i)
                if (aliveBlocks.insert(PN->getIncomingBlock(i)).second)
                    ++count;
    }
    return count;
}
