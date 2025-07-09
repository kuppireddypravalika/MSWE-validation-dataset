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
        // Optimize with early continue to skip unreachable blocks
        if (reachable.find(BB) == reachable.end()) continue;
        // Insert and count only if not already present
        if (aliveBlocks.emplace(BB).second) ++count;
        // Check for PHINode type to avoid unnecessary type checks
        if (PHINode* PN = dynamic_cast<PHINode*>(inst)) {
            // Reserve size for performance
            size_t numPreds = BB->preds.size();
            const std::vector<BasicBlock*> &preds = BB->preds;
            // Preallocate space in aliveBlocks for potential insertion
            for (size_t i = 0; i < numPreds; ++i) {
                BasicBlock* pred = preds[i];
                // Insert and count only if not already present
                if (aliveBlocks.emplace(pred).second) ++count;
            }
        }
    }
    return count;
}
