#include <chrono>
#include <iostream>
#include <string>

// forward declaration of target function
double process(int size);

int main(int argc, char* argv[]) {
    std::string mode = "perf";      // perf or correct
    int repeat = 8;
    int size = 256;  // number of elements (<1024 to expose issue)

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) size = std::stoi(arg.substr(7));
    }

    if (mode == "correct") {
        double val = process(size);
        std::cout << val << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) process(size);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
}
