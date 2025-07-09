#include "p256_helpers.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>

uint64_t p256_multiply(CodeGenerator& cg, const Vec16& A, const Vec16& B, int iters);

Vec16 make_vec() {
    std::mt19937 gen(42);
    std::uniform_int_distribution<uint32_t> dist(0, 1000);
    Vec16 v;
    for (auto& x : v) x = dist(gen);
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1100000;
    int iters = 6000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--iters=",0)==0) iters = std::stoi(arg.substr(8));
    }

    Vec16 A = make_vec();
    Vec16 B = make_vec();
    CodeGenerator cg(false); // vector registers unavailable

    if (mode == "correct") {
        uint64_t r = p256_multiply(cg, A, B, iters);
        std::cout << r << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        uint64_t dummy = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            dummy += p256_multiply(cg, A, B, iters);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
