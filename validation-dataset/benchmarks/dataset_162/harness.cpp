#include <chrono>
#include <iostream>
#include <string>

int for_function(int start, int end);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 5000; // high enough workload

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        int result = for_function(0, 100);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long sum = 0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            sum += for_function(0, 100);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (sum == 42) std::cerr << "";
    }
    return 0;
}
