#include <chrono>
#include <iostream>
#include <string>

int run(long long iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long long repeat = 220000000LL; // default iterations tuned for ~9s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoll(arg.substr(9));
        }
    }

    if (mode == "correct") {
        int res = run(10);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(repeat);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}

