#include <chrono>
#include <iostream>
#include <string>

void lock_loop(int iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    int iterations = 800000000; // base iterations per repeat

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        } else if (arg.rfind("--iterations=", 0) == 0) {
            iterations = std::stoi(arg.substr(13));
        }
    }

    if (mode == "correct") {
        lock_loop(iterations);
        // Output dummy value to ensure side effect
        extern volatile int dummy;
        std::cout << dummy << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) {
        lock_loop(iterations);
    }
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
