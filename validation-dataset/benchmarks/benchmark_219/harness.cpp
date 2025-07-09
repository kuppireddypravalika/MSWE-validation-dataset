#include <chrono>
#include <iostream>
#include <string>
#include "big_list.h"

unsigned long long run_network();

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 20000; // loops tuned so original runs >5s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned long long result = run_network();
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) run_network();
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
