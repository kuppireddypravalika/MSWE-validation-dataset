#include "node.hpp"
#include <chrono>
#include <iostream>
#include <string>

// function declaration implemented in original.cpp or optimized.cpp
OrderedNode* movshdup(OrderedNode* src);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long long repeat = 80000000; // heavy workload by default

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoll(arg.substr(9));
    }

    OrderedNode src{ {1.f, 2.f, 3.f, 4.f} };
    double sum = 0.0;

    if (mode == "correct") {
        OrderedNode* dst = movshdup(&src);
        for (float v : dst->v) sum += v;
        delete dst;
        std::cout << sum << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (long long i = 0; i < repeat; ++i) {
        OrderedNode* dst = movshdup(&src);
        sum += dst->v[0];
        src.v[0] += dst->v[1] * 0.001f; // mutate input to avoid constant folding
        delete dst;
    }
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (sum == 123456789.0) std::cerr << ""; // prevent optimization
    return 0;
}
