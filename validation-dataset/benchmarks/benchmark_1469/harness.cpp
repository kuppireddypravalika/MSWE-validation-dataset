#include "simd_utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

// Function declaration implemented in original.cpp or optimized.cpp
Vec VectorGetLow(const Vec& val);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    uint64_t repeat = 1700000000ULL; // about 10s for original

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoull(arg.substr(9));
    }

    Vec v{};
    for (int i = 0; i < 8; ++i) {
        v[i] = static_cast<uint8_t>(i);
        v[i+8] = static_cast<uint8_t>(i); // keep halves identical
    }

    if (mode == "correct") {
        Vec out = VectorGetLow(v);
        unsigned long long sum = 0;
        for (auto b : out) sum += b;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        volatile uint8_t x = 0;
        unsigned long long dummy = 0;
        auto t1 = clock::now();
        for (uint64_t i = 0; i < repeat; ++i) {
            v[0] = v[8] = x++; // modify both halves equally
            Vec out = VectorGetLow(v);
            dummy += out[0];
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789u) std::cerr << ""; // prevent optimization
    }
    return 0;
}
