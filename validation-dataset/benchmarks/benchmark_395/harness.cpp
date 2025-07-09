#include <chrono>
#include <iostream>
#include <string>
#include <cstdint>

long long analyze(uint32_t val, int repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 350000;     // workload control
    uint32_t val = 16;      // sample loop bound

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--val=", 0) == 0)
            val = static_cast<uint32_t>(std::stoul(arg.substr(6)));
    }

    if (mode == "correct") {
        long long result = analyze(val, 1);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        analyze(val, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
