#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <numeric>
#include "shared.h"

// Declarations from implementations
namespace original { template <class ELFT> class AtomSection; }
namespace optimized { template <class ELFT> class AtomSection; }

using Clock = std::chrono::high_resolution_clock;

// Function prototypes

float run(int repeat, size_t numAtoms, size_t atomSize);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 200; // number of write calls per run
    size_t atoms = 64; // 64 atoms * 1MB each = 64MB copied per call
    size_t atomSize = 1 << 20; // bytes

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        float result = run(1, atoms, atomSize);
        std::cout << result << std::endl;
        return 0;
    }

    auto time_func = [&](auto func){
        auto t1 = Clock::now();
        func();
        auto t2 = Clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    };

    auto elapsed = time_func([&]{ run(repeat, atoms, atomSize); });

    std::cout << "Time: " << elapsed << " ms\n";
    return 0;
}

// Structures shared with implementations are defined in shared.h

float checksum(const std::vector<uint8_t>& buf) {
    uint64_t sum = std::accumulate(buf.begin(), buf.end(), uint64_t{0});
    return static_cast<float>(sum & 0xFFFFFFFF);
}

