#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>

uint64_t run(size_t repeat, size_t elements);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t elements = 1000; // number of contained types
    size_t repeat = 9000000; // iterations tuned for >10s in original

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--elements=",0)==0) elements = std::stoull(arg.substr(11));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoull(arg.substr(9));
    }

    if (mode == "correct") {
        uint64_t result = run(1, elements);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        uint64_t dummy = run(repeat, elements);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
        if (dummy==123456789) std::cerr << ""; // prevent optimization
    }
}
