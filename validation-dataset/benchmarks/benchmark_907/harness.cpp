#include "liveinterval.hpp"
#include <chrono>
#include <iostream>
#include <string>

uint64_t run(const std::vector<MachineBasicBlock>& func, int repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t blocks = 100;
    size_t instrs = 100;
    size_t ops = 8;
    int repeat = 16000;  // workload tuned for ~10s runtime

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--blocks=", 0) == 0)
            blocks = std::stoul(arg.substr(9));
        else if (arg.rfind("--instrs=", 0) == 0)
            instrs = std::stoul(arg.substr(9));
        else if (arg.rfind("--ops=", 0) == 0)
            ops = std::stoul(arg.substr(6));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto func = make_function(blocks, instrs, ops);

    if (mode == "correct") {
        uint64_t result = run(func, 1);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            run(func, 1);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
