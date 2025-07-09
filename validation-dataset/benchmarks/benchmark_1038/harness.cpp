#include "thermo.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int size = 64;
    int repeat = 500;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=",0)==0)
            size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    Grid grid{size, size, size, 0, size, 0, size};
    size_t vol = static_cast<size_t>(grid.icells) * grid.jcells * grid.kcells;
    std::vector<double> b(vol), s(vol), qt(vol);
    std::vector<double> p(grid.kcells), ql(grid.icells * grid.jcells);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist_s(280.0, 310.0);
    std::uniform_real_distribution<double> dist_qt(0.001, 0.02);

    for (int k = 0; k < grid.kcells; ++k)
        p[k] = 100000.0 - 100.0 * k;
    for (size_t idx = 0; idx < vol; ++idx) {
        s[idx] = dist_s(gen);
        qt[idx] = dist_qt(gen);
    }

    if (mode == "correct") {
        calcbuoyancy(b.data(), s.data(), qt.data(), p.data(), ql.data(), grid);
        double sum = 0.0;
        for (double v : b) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        double dummy = 0.0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            calcbuoyancy(b.data(), s.data(), qt.data(), p.data(), ql.data(), grid);
            dummy += b[(r % vol)];
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
