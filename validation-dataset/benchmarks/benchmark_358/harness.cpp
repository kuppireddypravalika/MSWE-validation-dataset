#include "common.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::vector<DbgRecord> gRecords;

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t numInstrs = 4000;
    int repeat = 2500;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--instrs=", 0) == 0) numInstrs = std::stoul(arg.substr(9));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<std::unique_ptr<Instruction>> instructions;
    std::vector<std::unique_ptr<DbgInfoIntrinsic>> dbgInfos;
    instructions.reserve(numInstrs);
    dbgInfos.reserve(numInstrs);
    gRecords.reserve(numInstrs * 3 / 5);

    for (size_t i = 0; i < numInstrs; ++i) {
        bool used = (i % 5 == 0); // 20% have metadata
        instructions.emplace_back(std::make_unique<Instruction>(Instruction{used, static_cast<int>(i)}));
        Instruction* instPtr = instructions.back().get();
        if (used) {
            for (int j = 0; j < 3; ++j) {
                dbgInfos.emplace_back(std::make_unique<DbgInfoIntrinsic>());
                gRecords.push_back({instPtr, dbgInfos.back().get()});
            }
        }
    }

    if (mode == "correct") {
        for (auto& inst : instructions) salvageDebugInfo(*inst);
        unsigned long sum = 0;
        for (const auto& rec : gRecords) sum += rec.dbg->operand;
        std::cout << sum << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            for (auto& inst : instructions) salvageDebugInfo(*inst);
        }
        auto t2 = clock::now();
        unsigned long sum = 0;
        for (const auto& rec : gRecords) sum += rec.dbg->operand;
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (sum == 0xdeadbeef) std::cerr << sum; // prevent optimization
    }

    return 0;
}
