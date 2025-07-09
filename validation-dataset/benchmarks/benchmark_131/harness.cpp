#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

struct BitVector {
    std::vector<uint8_t> bits;
    void init(int32_t size) { bits.assign(size, 0); }
};

struct Compilation {
    int32_t symRefs;
    int32_t nodes;
    int32_t getSymRefCount() const { return symRefs; }
    int32_t getNodeCount() const { return nodes; }
    void incVisitCount() {}
};

struct LocalCSE {
    BitVector _seenCallSymbolReferences;
    BitVector _availableLoadExprs;
    BitVector _availablePinningArrayExprs;
    BitVector _availableCallExprs;
    BitVector _seenSymRefs;
    BitVector _possiblyRelevantNodes;
    BitVector _relevantNodes;
    BitVector _killedPinningArrayExprs;
    BitVector _killedNodes;
    BitVector _parentAddedToHT;
    std::vector<void*> _simulatedNodesAsArray;
    bool _mayHaveRemovedChecks = false;
    Compilation* _comp;
    LocalCSE(Compilation* c) : _comp(c) {}
    Compilation* comp() { return _comp; }
};

void prePerformOnBlocks(LocalCSE& self);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int32_t symRefs = 50000;
    int32_t nodes = 50000;
    int repeat = 40000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--symrefs=",0)==0) symRefs = std::stoi(arg.substr(10));
        else if (arg.rfind("--nodes=",0)==0) nodes = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        Compilation comp{symRefs, nodes};
        LocalCSE cse(&comp);
        prePerformOnBlocks(cse);
        std::cout << cse._simulatedNodesAsArray.size() << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (int r = 0; r < repeat; ++r) {
        Compilation comp{symRefs, nodes};
        LocalCSE cse(&comp);
        prePerformOnBlocks(cse);
    }
    auto t2 = clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    return 0;
}
