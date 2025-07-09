#include <chrono>
#include <iostream>
#include <string>

extern "C" double run(int repeat, size_t stackSize);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 50;         // number of outer iterations
    size_t size = 80000;     // stack slots to process

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=",0)==0) size = std::stoul(arg.substr(7));
    }

    if (mode == "correct") {
        double result = run(1, size);
        std::cout << result << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    run(repeat, size);
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
