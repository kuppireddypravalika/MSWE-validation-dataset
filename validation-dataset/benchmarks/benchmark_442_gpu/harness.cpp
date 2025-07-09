#include "grid.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <omp.h>

void operation_mult(DataVector& alpha, DataVector& result, const GridStorage& storage, int degree);

GridStorage make_grid(size_t size, size_t dim) {
    GridStorage g(size, dim);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> lvl(1, 4);
    for (size_t i = 0; i < size; ++i) {
        for (size_t d = 0; d < dim; ++d) {
            int l = lvl(gen);
            g[i].level[d] = l;
            std::uniform_int_distribution<int> idx(0, (1 << l) - 1);
            g[i].index[d] = idx(gen);
        }
    }
    return g;
}

DataVector make_vector(size_t size) {
    DataVector v(size);
    std::mt19937 gen(1337);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (size_t i = 0; i < size; ++i) v[i] = dist(gen);
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 60;
    size_t dim = 3;
    int degree = 3;
    int repeat = 21000;
    int threads = std::min(4, omp_get_max_threads());

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = static_cast<size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--dim=",0)==0) dim = static_cast<size_t>(std::stoul(arg.substr(6)));
        else if (arg.rfind("--degree=",0)==0) degree = std::stoi(arg.substr(9));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
    }
    if (mode == "correct")
        omp_set_num_threads(1);
    else
        omp_set_num_threads(threads);

    GridStorage storage = make_grid(size, dim);
    DataVector alpha = make_vector(size);
    DataVector result(size);

    if (mode == "correct") {
        operation_mult(alpha, result, storage, degree);
        double sum = 0.0;
        for (double v : result.data) sum += v;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            operation_mult(alpha, result, storage, degree);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
