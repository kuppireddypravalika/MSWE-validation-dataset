#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "licm_sim.hpp"

void hoist(Instruction& I, BasicBlock& Preheader, AliasSetTracker& CurAST);

size_t run(int insts) {
    BasicBlock preheader("preheader");
    BasicBlock body("body");
    AliasSetTracker ast;
    std::vector<Instruction*> vec;
    vec.reserve(insts);
    for (int i = 0; i < insts; ++i) {
        Instruction* in = new Instruction(i);
        body.addInstruction(in);
        ast.addValue(in);
        vec.push_back(in);
    }
    for (auto* in : vec) hoist(*in, preheader, ast);
    size_t sum = 0;
    for (Instruction* I : preheader.insts) {
        if (I != preheader.getTerminator()) sum += I->value;
    }
    return sum;
}


int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 45000;
    int insts = 2000;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--insts=",0)==0) insts = std::stoi(arg.substr(8));
    }
    if (mode == "correct") {
        size_t res = run(insts);
        std::cout << res << "\n";
        return 0;
    }
    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int r=0;r<repeat;++r) run(insts);
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " ms\n";
}
