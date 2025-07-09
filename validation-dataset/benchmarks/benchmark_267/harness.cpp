#include <chrono>
#include <iostream>
#include <string>

size_t run(size_t N);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t repeat = 100;
    size_t size = 5200; // number of atoms in chain

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoul(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
    }

    if (mode == "correct") {
        size_t result = run(size);
        std::cout << result << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (size_t i = 0; i < repeat; ++i)
        run(size);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";

    return 0;
}
