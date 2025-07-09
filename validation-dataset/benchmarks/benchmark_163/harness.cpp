#include <chrono>
#include <iostream>
#include <string>

std::string locate_bundler();

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original";
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--impl=",0) == 0)
            impl = arg.substr(7);
        else if (arg.rfind("--repeat=",0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        std::string result = locate_bundler();
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            locate_bundler();
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
