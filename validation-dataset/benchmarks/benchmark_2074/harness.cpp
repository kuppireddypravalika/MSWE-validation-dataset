#include <chrono>
#include <iostream>
#include <string>

uint64_t run(size_t repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t repeat = 20000000; // heavy default for perf

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoull(arg.substr(9));
    }

    if (mode == "correct") {
        repeat = 1000; // small for correctness
        uint64_t result = run(repeat);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        uint64_t dummy = run(repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << ""; // prevent optimization
    }
    return 0;
}
