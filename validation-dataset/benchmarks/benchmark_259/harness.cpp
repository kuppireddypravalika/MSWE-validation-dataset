#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "region.hpp"
bool isRegion(BasicBlock* entry, BasicBlock* exit,
              const std::vector<BasicBlock*>& exitSuccs,
              const DominatorTree& DT);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int blocks = 50;
    int succs = 1000;
    int repeat = 1800;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--blocks=", 0) == 0)
            blocks = std::stoi(arg.substr(9));
        else if (arg.rfind("--succs=", 0) == 0)
            succs = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::vector<BasicBlock> nodes(blocks);
    std::vector<BasicBlock*> ptrs(blocks);
    for (int i = 0; i < blocks; ++i) {
        nodes[i].id = i;
        ptrs[i] = &nodes[i];
    }
    BasicBlock* entry = &nodes[0];
    BasicBlock* exit = &nodes[1];
    DominatorTree DT(ptrs, entry);

    std::vector<BasicBlock*> exitSuccs;
    exitSuccs.reserve(succs);
    for (int i = 0; i < succs; ++i) {
        if (i % 2 == 0)
            exitSuccs.push_back(entry); // triggers expensive dominates check
        else
            exitSuccs.push_back(ptrs[(i % (blocks - 2)) + 2]);
    }

    if (mode == "correct") {
        bool result = isRegion(entry, exit, exitSuccs, DT);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            isRegion(entry, exit, exitSuccs, DT);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
