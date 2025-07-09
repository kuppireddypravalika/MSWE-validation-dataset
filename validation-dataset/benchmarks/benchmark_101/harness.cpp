#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

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

size_t adce(const std::vector<Instruction*>&, const std::unordered_set<BasicBlock*>&, std::unordered_set<BasicBlock*>&);

struct CFG {
    std::vector<BasicBlock> blocks;
    std::vector<PHINode> phis;
    std::vector<Instruction*> worklist;
    std::unordered_set<BasicBlock*> reachable;
};

CFG create_cfg(size_t nBlocks, size_t preds) {
    CFG cfg;
    cfg.blocks.resize(nBlocks);
    cfg.phis.reserve(nBlocks);
    cfg.worklist.reserve(nBlocks);
    for (size_t i = 0; i < nBlocks; ++i) {
        cfg.blocks[i].preds.reserve(preds);
    }
    for (size_t i = 0; i < nBlocks; ++i) {
        for (size_t j = 0; j < preds; ++j)
            cfg.blocks[i].preds.push_back(&cfg.blocks[(i + j + 1) % nBlocks]);
    }
    for (size_t i = 0; i < nBlocks; ++i) {
        cfg.phis.emplace_back();
        cfg.phis.back().parent = &cfg.blocks[i];
        cfg.phis.back().incoming = cfg.blocks[i].preds;
        cfg.worklist.push_back(&cfg.phis.back());
    }
    for (auto& b : cfg.blocks) cfg.reachable.insert(&b);
    return cfg;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t blocks = 1000;
    int repeat = 90000;
    size_t preds = 8;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--blocks=", 0) == 0) blocks = std::stoul(arg.substr(9));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    CFG cfg = create_cfg(blocks, preds);
    std::unordered_set<BasicBlock*> alive;

    if (mode == "correct") {
        size_t result = adce(cfg.worklist, cfg.reachable, alive);
        std::cout << result << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            adce(cfg.worklist, cfg.reachable, alive);
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
