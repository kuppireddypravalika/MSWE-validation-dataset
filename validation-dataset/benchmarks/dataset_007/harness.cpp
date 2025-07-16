#include "ripser_stub.hpp"
#include <chrono>
#include <iostream>
#include <string>

long long run(const sparse_distance_matrix& dist, int repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 20000000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto dist = make_matrix(20);

    if (mode == "correct") {
        long long sum = run(dist, 1);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        long long sum = run(dist, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (sum == 42) std::cerr << ""; // prevent optimization
    }
    return 0;
}
