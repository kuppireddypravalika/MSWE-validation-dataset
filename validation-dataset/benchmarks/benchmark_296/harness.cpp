#include "osr_sim.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

long long run(Compilation& comp, std::vector<Instruction>& instrs, int repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int instructions = 1000;
    int repeat = 900;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--instr=",0) == 0)
            instructions = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::vector<SymbolReference> syms(instructions/10);
    std::vector<Node> nodes(instructions);
    std::vector<Instruction> instrs(instructions);
    for (int i = 0; i < instructions; ++i) {
        SymbolReference* sym = nullptr;
        if (i % 10 == 0) {
            syms[i/10].ref = TR_induceOSRAtCurrentPC;
            sym = &syms[i/10];
        }
        nodes[i].sym = sym;
        instrs[i].node = &nodes[i];
        instrs[i].pc = i;
        instrs[i].bcInfo = {0, i};
    }

    Compilation comp(voluntaryOSR);
    long long result = 0;

    if (mode == "correct") {
        result = run(comp, instrs, repeat);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(comp, instrs, repeat);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
}
