#include <chrono>
#include <iostream>
#include <string>

size_t run(size_t loops);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t loops = 80000; // workload to exceed 5s in original
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=", 0) == 0) loops = std::stoull(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        size_t small = 10; // light workload for correctness
        std::cout << run(small) << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) run(loops);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
