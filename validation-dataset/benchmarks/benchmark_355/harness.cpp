#include <chrono>
#include <iostream>
#include <string>

uint64_t run(size_t entries, size_t loops);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t entries = 1024;
    size_t loops = 5000; // will adjust to ensure >10s for original
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--entries=", 0) == 0) entries = std::stoull(arg.substr(10));
        else if (arg.rfind("--loops=", 0) == 0) loops = std::stoull(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        uint64_t res = run(entries, 1);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) run(entries, loops);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
}
