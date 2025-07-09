#include "burned_warehouse.hpp"
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 9000000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    BurnedWarehouse base({50, 0, 0, 0, 50});

    if (mode == "correct") {
        BurnedWarehouse bw = base;
        int result = handle_event(bw);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        int dummy = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            BurnedWarehouse bw = base;
            dummy += handle_event(bw);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 42) std::cerr << "";
    }

    return 0;
}
