#include <chrono>
#include <iostream>
#include <string>

struct Module {
    long long loops;
    bool canonical = false;
    bool polyOptimized = false;
};

double execute(Module m);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long long loops = 350000000LL; // workload per execution
    int repeat = 1; // ensures runtime >10s on most systems

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=",0)==0) loops = std::stoll(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    Module base{loops};

    if (mode == "correct") {
        Module m = base;
        double result = execute(m);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i=0;i<repeat;++i) {
            Module m = base;
            execute(m);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
