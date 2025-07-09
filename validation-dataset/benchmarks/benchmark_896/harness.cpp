#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>

// Function implemented in original.cpp or optimized.cpp
unsigned compute_mipmap_offset(unsigned width);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    unsigned width = 1024;
    long long repeat = 700000000LL; // heavy workload to ensure >5s for original

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--width=", 0) == 0)
            width = static_cast<unsigned>(std::stoul(arg.substr(8)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoll(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned result = compute_mipmap_offset(width);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        volatile unsigned dummy = 0;
        auto t1 = clock::now();
        for (long long i = 0; i < repeat; ++i) {
            dummy += compute_mipmap_offset(width + (i & 1023));
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 0x12345678u) std::cerr << "";
    }
    return 0;
}
