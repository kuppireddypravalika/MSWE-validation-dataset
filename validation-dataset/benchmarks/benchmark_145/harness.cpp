#include <chrono>
#include <iostream>
#include <string>

// Declarations from the two implementations

double compute(int nnodes, int loops);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int nnodes = 50021;          // large prime >46k
    int loops = 100000000;       // workload per call
    int repeat = 40;             // number of calls

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--loops=",0) == 0)
            loops = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        loops = 0;       // skip expensive work during correctness run
        repeat = 1;
        double result = compute(nnodes, loops);
        std::cout << result << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    volatile double sink = 0;  // prevent optimization

    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i)
        sink += compute(nnodes, loops);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    // use sink so compiler cannot optimize away
    if (sink == 0.123) std::cerr << sink; // never executed
}
