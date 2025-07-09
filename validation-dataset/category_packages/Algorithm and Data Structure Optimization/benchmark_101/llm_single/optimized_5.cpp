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
        // Attempt to insert BB into aliveBlocks; if successful, increment count.
        if (aliveBlocks.insert(BB).second) { 
            ++count;
        }
        // Use dynamic_cast only if inst is a PHINode, otherwise avoid overhead.
        if (auto* PN = dynamic_cast<PHINode*>(inst)) {
            // Reserve space in aliveBlocks if the block will have many predecessors.
            const auto& preds = BB->preds;
            for (BasicBlock* pred : preds) {
                // Directly check and insert in one operation to avoid checking twice.
                if (aliveBlocks.insert(pred).second) {
                    ++count;
                }
            }
        }
    }
    return count;
}
