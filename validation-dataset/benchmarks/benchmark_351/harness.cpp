#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "bitvector.h"

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 200;
    std::size_t bits = static_cast<std::size_t>(64) * 1024 * 1024 * 8; // 64MB

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--bits=",0)==0) bits = std::stoull(arg.substr(7));
    }

    BitVector a(bits);
    BitVector b(bits);

    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0,255);
    for(auto& by : a.MutableData()) by = std::byte(dist(gen));
    for(auto& by : b.MutableData()) by = std::byte(dist(gen));

    if (mode == "correct") {
        a ^= b;
        uint64_t sum = 0;
        for (auto by : a.GetData()) sum += std::to_integer<unsigned char>(by);
        std::cout << sum << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(int i=0;i<repeat;++i) {
            a ^= b;
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
    return 0;
}
