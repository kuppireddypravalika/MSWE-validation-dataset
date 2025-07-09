#include <chrono>
#include <iostream>
#include <string>

uint64_t translation_cache_sum(int loops, size_t kb);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int loops = 23000;
    size_t kb = 512;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=", 0) == 0) loops = std::stoi(arg.substr(8));
        else if (arg.rfind("--size=", 0) == 0) kb = std::stoul(arg.substr(7));
    }

    if (mode == "correct") {
        uint64_t result = translation_cache_sum(1, kb);
        std::cout << result << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    translation_cache_sum(loops, kb);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
