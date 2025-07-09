#include <chrono>
#include <iostream>
#include <string>

unsigned long push_and_sum(unsigned count);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    unsigned count = 100000;
    int repeat = 15;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--count=", 0) == 0)
            count = static_cast<unsigned>(std::stoul(arg.substr(8)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        unsigned long sum = push_and_sum(count);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            push_and_sum(count);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
