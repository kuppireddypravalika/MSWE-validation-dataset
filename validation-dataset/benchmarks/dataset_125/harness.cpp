#include "header.hpp"
#include <chrono>
#include <iostream>
#include <string>

long long run(long long iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original"; // parsed for compatibility
    long long repeat = 1000000; // tuned for ~10s with original

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--impl=",0)==0)
            impl = arg.substr(8);
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoll(arg.substr(9));
    }

    if (mode == "correct") {
        long long res = run(10);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        long long dummy = run(repeat);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (dummy == 123456789) std::cerr << ""; // prevent optimization
    }
    return 0;
}
