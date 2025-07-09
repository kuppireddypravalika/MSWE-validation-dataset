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
    std::unordered_set<BasicBlock*> tempSet;

    for (Instruction* inst : workList) {
        BasicBlock* BB = inst->parent;
        if (!reachable.count(BB)) continue;
        if (tempSet.insert(BB).second) {
            count++;
            aliveBlocks.insert(BB);
        }

        if (PHINode* PN = dynamic_cast<PHINode*>(inst)) {
            for (BasicBlock* pred : BB->preds) {
                if (tempSet.insert(pred).second) {
                    count++;
                    aliveBlocks.insert(pred);
                }
            }
        }
    }

    aliveBlocks.insert(tempSet.begin(), tempSet.end());
    return count;
}

// Explicit template instantiation statements (if present) should be placed here.
