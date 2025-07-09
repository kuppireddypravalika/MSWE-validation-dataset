#include <chrono>
#include <iostream>
#include <string>

uint64_t run(size_t iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t iterations = 1000; // default used in perf mode
    int repeat = 5500;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--iterations=", 0) == 0) iterations = std::stoull(arg.substr(14));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        uint64_t result = run(10); // small for correctness
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
