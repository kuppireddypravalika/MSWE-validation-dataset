#include "common.h"
#include <chrono>
#include <iostream>
#include <random>
#include <string>

// Function declarations

double information_theoretic_bits(const field_set& fs);

// Generate a field_set with given counts
field_set generate_fs(size_t count) {
    field_set fs;
    for (size_t i = 0; i < count; ++i) {
        fs.discs.push_back({65536});
        fs.contins.push_back({16});
        fs.ontos.push_back({65536, 16});
    }
    return fs;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 20000; // number of elements in each vector
    int repeat = 70;     // number of iterations

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    field_set fs = generate_fs(size);

    if (mode == "correct") {
        double res = information_theoretic_bits(fs);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            information_theoretic_bits(fs);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
