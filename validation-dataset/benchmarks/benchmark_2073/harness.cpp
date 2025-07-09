#include <chrono>
#include <iostream>
#include <string>

uint64_t run(size_t repeat, uint32_t end);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t repeat = 2;
    uint32_t end = 0; // value passed to the counting functions

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoull(arg.substr(9));
        else if (arg.rfind("--end=",0)==0) end = static_cast<uint32_t>(std::stoul(arg.substr(6)));
    }

    if (mode == "correct") {
        repeat = 1;
        if (end == 0) end = 1073741824; // choose value yielding same max
        uint64_t result = run(repeat, end);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        uint64_t dummy = run(repeat, end);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
