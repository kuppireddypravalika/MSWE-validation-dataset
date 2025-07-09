#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "bench.hpp"

int run_block(int num_vars, int repeat);

int main(int argc, char** argv) {
    std::string mode = "perf";
    int vars = 5000;
    int repeat = 45000;

    for (int i=1; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--vars=",0)==0) vars = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        int r = run_block(vars, repeat);
        std::cout << r << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        int r = run_block(vars, repeat);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
        (void)r;
    }
    return 0;
}
