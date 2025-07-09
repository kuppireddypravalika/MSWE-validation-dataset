#include <chrono>
#include <iostream>
#include <string>

// The function 'run' is provided by either original.cpp or optimized.cpp
long long run(unsigned long long loops);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    unsigned long long loops = 6000000000ULL; // heavy work for performance test
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=",0) == 0) loops = std::stoull(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned long long test_loops = 1000; // fast check
        long long result = run(test_loops);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            run(loops);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
