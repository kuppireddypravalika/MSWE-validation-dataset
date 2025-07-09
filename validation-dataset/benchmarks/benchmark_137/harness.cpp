#include <chrono>
#include <iostream>
#include <string>

unsigned long long process(std::size_t iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t iterations = 150000; // default workload
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--iterations=", 0) == 0)
            iterations = std::stoull(arg.substr(13));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned long long sum = process(iterations);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            process(iterations);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
