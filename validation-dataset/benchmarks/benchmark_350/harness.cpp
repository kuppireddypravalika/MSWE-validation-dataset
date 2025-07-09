#include <chrono>
#include <iostream>
#include <string>

int argcast_sim(int numFuncs, int callsPerFunc, int iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int funcs = 1000;
    int calls = 100;
    int iters = 330;
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--funcs=", 0) == 0)
            funcs = std::stoi(arg.substr(8));
        else if (arg.rfind("--calls=", 0) == 0)
            calls = std::stoi(arg.substr(8));
        else if (arg.rfind("--iters=", 0) == 0)
            iters = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        int res = argcast_sim(funcs, calls, iters);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        int res = 0;
        for (int r = 0; r < repeat; ++r)
            res += argcast_sim(funcs, calls, iters);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
