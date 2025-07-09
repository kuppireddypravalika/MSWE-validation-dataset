#include <chrono>
#include <iostream>
#include <string>
#include <cstddef>

// functions from original.cpp and optimized.cpp

double process(std::size_t loops);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 12;
    std::size_t loops = 50000000; // heavy work per open

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--loops=", 0) == 0)
            loops = static_cast<std::size_t>(std::stoull(arg.substr(8)));
    }

    if (mode == "correct") {
        double result = process(loops);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            process(loops);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
