#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Function { bool optNone; Function(bool o=true):optNone(o){} };
struct CallGraphNode { Function* F; Function* getFunction() const { return F; } };
using CallGraphSCC = std::vector<CallGraphNode*>;

bool runImpl(CallGraphSCC &SCC);

CallGraphSCC createOptNoneSCC(size_t count) {
    CallGraphSCC scc;
    for (size_t i = 0; i < count; ++i) {
        Function* F = new Function(true);
        CallGraphNode* N = new CallGraphNode{F};
        scc.push_back(N);
    }
    return scc;
}

void cleanupSCC(CallGraphSCC& scc) {
    for (auto* N : scc) {
        delete N->F;
        delete N;
    }
    scc.clear();
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t nodes = 10;
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--nodes=",0)==0) nodes = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    CallGraphSCC scc = createOptNoneSCC(nodes);

    if (mode == "correct") {
        bool changed = runImpl(scc);
        std::cout << changed << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i=0; i<repeat; ++i) runImpl(scc);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    cleanupSCC(scc);
    return 0;
}

