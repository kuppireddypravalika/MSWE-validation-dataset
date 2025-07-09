#ifndef REGION_HPP
#define REGION_HPP

#include <vector>
#include <cstddef>

struct BasicBlock { int id; };

class DominatorTree {
public:
    DominatorTree(const std::vector<BasicBlock*>& blocks, BasicBlock* entry)
        : domSets(blocks.size()), Entry(entry) {
        for (BasicBlock* b : blocks)
            domSets[Entry->id].push_back(b);
    }

    bool dominates(BasicBlock* a, BasicBlock* b) const {
        volatile bool result = false;
        const auto& vec = domSets[a->id];
        for (int iter = 0; iter < HeavyIter; ++iter) {
            for (std::size_t i = 0; i < vec.size(); ++i) {
                if (vec[i] == b)
                    result = true;
            }
        }
        return result;
    }

    bool properlyDominates(BasicBlock* a, BasicBlock* b) const {
        if (a == b) return false;
        return dominates(a, b);
    }

private:
    std::vector<std::vector<BasicBlock*>> domSets;
    BasicBlock* Entry;
    static const int HeavyIter = 90000;
};

#endif // REGION_HPP
