#include "shared.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 10000;
    int repeat = 100000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=",0)==0)
            size = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    RifColumnBasedUserData data(size);

    if (mode == "correct") {
        std::vector<double> vals;
        data.values(0, &vals);
        double sum = 0.0;
        for (double v : vals) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        double dummy = 0.0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            std::vector<double> vals;
            data.values(0, &vals);
            dummy += vals.empty() ? 0.0 : vals[0];
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789.0) std::cerr << "";
    }
    return 0;
}
