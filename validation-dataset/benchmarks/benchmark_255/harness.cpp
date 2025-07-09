#include <chrono>
#include <iostream>
#include <string>

size_t run_algorithm(int iterations, int items);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int iterations = 470000; // workload
    int items = 100;
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--iterations=", 0) == 0) iterations = std::stoi(arg.substr(13));
        else if (arg.rfind("--items=", 0) == 0) items = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        size_t sum = run_algorithm(iterations, items);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            run_algorithm(iterations, items);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
