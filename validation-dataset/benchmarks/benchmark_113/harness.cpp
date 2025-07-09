#include <chrono>
#include <iostream>
#include <string>

namespace demo {
using IntFunc = uint64_t(*)(int);
template<typename F> F parseFile(const std::string&, const std::string&);
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 8000;
    int size = 2000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        } else if (arg.rfind("--size=", 0) == 0) {
            size = std::stoi(arg.substr(7));
        }
    }

    using demo::IntFunc;
    if (mode == "correct") {
        IntFunc f = demo::parseFile<IntFunc>("", "func");
        std::cout << f(size) << std::endl;
    } else {
        IntFunc fo = demo::parseFile<IntFunc>("", "func");
        IntFunc fp = demo::parseFile<IntFunc>("", "func");
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) fo(size);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
