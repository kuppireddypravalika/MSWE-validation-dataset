#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

void computeCollisions(size_t num_threads,
                       const std::vector<std::vector<double>>& q,
                       std::vector<bool>& res,
                       bool stopAtFirstCollisionInConfiguration = false,
                       bool stopAtFirstCollisionInBatch = false);

static std::vector<std::vector<double>> createInput(size_t batch, size_t dim) {
    std::vector<std::vector<double>> q(batch, std::vector<double>(dim, 0.1));
    return q;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t batch = 1000000;
    size_t dim = 4;
    int repeat = 20;
    int threads = std::min(4, omp_get_max_threads());

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--batch=", 0) == 0) batch = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=", 0) == 0) threads = std::stoi(arg.substr(10));
    }

    std::vector<std::vector<double>> q = createInput(batch, dim);
    std::vector<bool> res(batch);

    if (mode == "correct") {
        computeCollisions(threads, q, res);
        size_t sum = 0;
        for (bool v : res) if (v) ++sum;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) computeCollisions(threads, q, res);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
