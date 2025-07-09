#include <chrono>
#include <iostream>
#include <string>
#include "thread_mock.h"

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t iterations = 1000000;
    int repeat = 14000;
    size_t threads = 1000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--iterations=", 0) == 0) iterations = std::stoull(arg.substr(14));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=", 0) == 0) threads = std::stoull(arg.substr(10));
    }

    init_threads(threads);

    if (mode == "correct") {
        uint64_t result = run(10);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) run(iterations);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
