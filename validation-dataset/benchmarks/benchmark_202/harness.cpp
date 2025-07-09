#include <chrono>
#include <iostream>
#include <string>

int process(int iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 90000000; // default workload

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        int out = process(1);
        std::cout << out << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    int res = process(repeat);
    auto t2 = clock::now();

    // ensure result used to prevent optimization out
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    if (res == 0) std::cerr << ""; // use result

    return 0;
}
