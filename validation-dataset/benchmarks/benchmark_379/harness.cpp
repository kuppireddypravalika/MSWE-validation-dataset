#include <chrono>
#include <iostream>
#include <string>

int run(int iterations, int classes);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    int iterations = 500;
    int classes = 200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--iterations=",0) == 0) iterations = std::stoi(arg.substr(13));
        else if (arg.rfind("--classes=",0) == 0) classes = std::stoi(arg.substr(10));
    }

    if (mode == "correct") {
        std::cout << run(1, 5) << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) run(iterations, classes);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
