#include "common.hpp"
#include <chrono>
#include <iostream>
#include <string>

int run_pass(const RegisterState& regs, int repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int size = 10000;
    int repeat = 15;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    RegisterState regs;
    regs.reserve(size);
    for (int i = 0; i < size; ++i)
        regs.emplace_back(i, Expression(i + 1));

    if (mode == "correct") {
        int sum = run_pass(regs, 1);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run_pass(regs, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
