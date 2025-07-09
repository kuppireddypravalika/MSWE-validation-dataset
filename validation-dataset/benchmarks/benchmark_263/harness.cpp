#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

struct MlOptimiser {};

void expectationSomeParticles(MlOptimiser* self, int nr_threads, int tasks, std::vector<double>& results);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int nr_threads = std::min(4, omp_get_max_threads());
    int tasks = 55000;
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--threads=", 0) == 0)
            nr_threads = std::stoi(arg.substr(10));
        else if (arg.rfind("--tasks=", 0) == 0)
            tasks = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    MlOptimiser mlo;
    std::vector<double> results;

    if (mode == "correct") {
        expectationSomeParticles(&mlo, nr_threads, tasks, results);
        double sum = 0.0;
        for (double v : results) sum += v;
        std::cout << sum << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            expectationSomeParticles(&mlo, nr_threads, tasks, results);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
