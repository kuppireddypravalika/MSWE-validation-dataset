#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>

using AttributeMap = std::unordered_map<std::string, int>;
struct OperatorWithKernel {
    AttributeMap attrs;
    const AttributeMap& Attrs() const { return attrs; }
};

extern int prepare_op(OperatorWithKernel& op, const AttributeMap& new_attrs);

// Definition of the global flag used in optimized.cpp
bool FLAGS_use_mkldnn = false;

// Helper to create a map with deterministic contents
AttributeMap make_attrs(int size) {
    AttributeMap m;
    for (int i = 0; i < size; ++i) {
        m["attr" + std::to_string(i)] = i;
    }
    return m;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int size = 50000;   // number of attributes
    int repeat = 7000;  // iterations for perf mode
    FLAGS_use_mkldnn = false; // default

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg == "--use_mkldnn") FLAGS_use_mkldnn = true;
    }

    AttributeMap initial = make_attrs(size);
    AttributeMap new_attrs = make_attrs(size);
    OperatorWithKernel op{initial};

    if (mode == "correct") {
        int sum = prepare_op(op, new_attrs);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            prepare_op(op, new_attrs);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
