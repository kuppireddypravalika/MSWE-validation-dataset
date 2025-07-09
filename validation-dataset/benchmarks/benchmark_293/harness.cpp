#include <chrono>
#include <iostream>
#include <string>

unsigned long run(int repeat, int str_size);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 13000000; // number of operations
    int size = 10000;      // string length

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) size = std::stoi(arg.substr(7));
    }

    if (mode == "correct") {
        unsigned long result = run(repeat, size);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(repeat, size);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
