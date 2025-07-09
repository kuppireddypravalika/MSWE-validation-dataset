#include <chrono>
#include <iostream>
#include <string>

long long distref_simulate(int iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    int iterations = 500000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--iterations=", 0) == 0) iterations = std::stoi(arg.substr(13));
    }

    if (mode == "correct") {
        long long result = distref_simulate(iterations);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            distref_simulate(iterations);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
