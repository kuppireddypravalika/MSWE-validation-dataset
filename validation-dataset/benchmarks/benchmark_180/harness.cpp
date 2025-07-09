#include <chrono>
#include <iostream>
#include <string>

// Target function implemented by either version
double run_benchmark(int n, int inner_repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";      // "perf" or "correct"
    int outer_repeat = 1;           // repetitions for timing
    int size = 512;                 // matrix dimension
    int inner_repeat = 13;          // workload inside each call

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) outer_repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) size = std::stoi(arg.substr(7));
        else if (arg.rfind("--inner=", 0) == 0) inner_repeat = std::stoi(arg.substr(8));
    }

    if (mode == "correct") {
        double res = run_benchmark(size, inner_repeat);
        std::cout << res << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int i = 0; i < outer_repeat; ++i) run_benchmark(size, inner_repeat);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
}
