#include <chrono>
#include <iostream>
#include <string>
#include "types.h"

void dropAllTypeUses(DerivedType& DT); // implemented in selected file

// Shared sink variable
volatile std::uintptr_t sink = 0;

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long long repeat = 6000;
    std::size_t num = 1000000; // number of contained types

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoll(arg.substr(9));
        else if (arg.rfind("--size=",0) == 0) num = std::stoull(arg.substr(7));
    }

    Context Ctx;
    DerivedType DT(Ctx, num);

    if (mode == "correct") {
        dropAllTypeUses(DT);
        std::size_t same = 0;
        for (std::size_t i = 0; i < DT.NumContainedTys; ++i)
            if (DT.ContainedTys[i] == DT.ContainedTys[0])
                ++same;
        std::cout << same << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (long long r = 0; r < repeat; ++r) {
            dropAllTypeUses(DT);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
