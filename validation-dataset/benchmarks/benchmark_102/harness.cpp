#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

int copy_params_sum(const int* params, int count);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    // Increase workload so the original version runs for at least 10s
    // on faster CI machines. This value yields ~11s locally.
    int repeat = 360000000;
    int count = 32;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--count=", 0) == 0)
            count = std::stoi(arg.substr(8));
    }

    std::vector<int> params(count);
    for (int i = 0; i < count; ++i) params[i] = i + 1;

    if (mode == "correct") {
        int res = copy_params_sum(params.data(), count);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            copy_params_sum(params.data(), count);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
