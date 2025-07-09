#include <chrono>
#include <iostream>
#include <string>

// Declaration of the benchmark function implemented in original.cpp or optimized.cpp
unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int size = 60;      // workload dimension
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned int res = CalculateTwoElectronIntegrals(false, size);
        std::cout << res << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    unsigned int dummy = 0;
    for (int r = 0; r < repeat; ++r) {
        dummy += CalculateTwoElectronIntegrals(true, size);
    }
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (dummy == 123456789u) std::cerr << "";
    return 0;
}
