#include <chrono>
#include <iostream>
#include <string>

int run(int repeat, int loops);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1000000;
    int loops = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--loops=",0)==0) loops = std::stoi(arg.substr(8));
    }

    if (mode == "correct") {
        int result = run(1, 10);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(repeat, loops);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
