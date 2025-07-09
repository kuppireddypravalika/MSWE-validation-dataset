#include <chrono>
#include <iostream>
#include <string>

long long run_algorithm(std::size_t N);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 200;
    int repeat = 150;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=",0)==0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        long long r = run_algorithm(size);
        std::cout << r << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) run_algorithm(size);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
}
