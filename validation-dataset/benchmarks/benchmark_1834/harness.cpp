#include "matrix.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>

// function defined in original.cpp or optimized.cpp

double jacobian_product(const Matrix& J);

Matrix make_matrix(std::size_t points) {
    std::size_t cols = 2 * points;
    Matrix J(6, cols);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (std::size_t i = 0; i < J.rows; ++i)
        for (std::size_t j = 0; j < J.cols; ++j)
            J(i, j) = dist(gen);
    return J;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t points = 200;
    long long repeat = 700000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--points=", 0) == 0)
            points = static_cast<std::size_t>(std::stoul(arg.substr(9)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoll(arg.substr(9));
    }

    Matrix J = make_matrix(points);

    if (mode == "correct") {
        double res = jacobian_product(J);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        double dummy = 0.0;
        auto t1 = clock::now();
        for (long long r = 0; r < repeat; ++r) {
            dummy += jacobian_product(J);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789.0) std::cerr << "";
    }
    return 0;
}

