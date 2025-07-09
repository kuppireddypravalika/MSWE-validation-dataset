#include <chrono>
#include <iostream>
#include <string>

// Target function implemented by either version
size_t run(int repeat, size_t entries, size_t leaves, size_t work);

int main(int argc, char** argv) {
    std::string mode = "perf";
    int repeat = 240;
    size_t entries = 100;
    size_t leaves = 50;
    size_t work = 32000000ULL; // loop iterations inside GetEntries

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--entries=",0)==0) entries = std::stoull(arg.substr(10));
        else if (arg.rfind("--leaves=",0)==0) leaves = std::stoull(arg.substr(9));
        else if (arg.rfind("--work=",0)==0) work = std::stoull(arg.substr(7));
    }

    if (mode == "correct") {
        size_t result = run(1, entries, leaves, work);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(repeat, entries, leaves, work);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
