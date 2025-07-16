#include <chrono>
#include <iostream>
#include <string>

extern "C" double run(int repeat, size_t size);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 250000;       // workload to exceed 10s in original
    size_t size = 1000;        // number of peaks

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
    }

    if (mode == "correct") {
        double out = run(1, size);
        std::cout << out << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    double val = run(repeat, size);
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (val == 123456.789) std::cerr << val;
    return 0;
}
