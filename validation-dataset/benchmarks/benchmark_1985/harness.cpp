#include "tank.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using state_type = std::vector<double>;

void step(const state_type&, state_type&, double, const TankData&);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original"; // parsed for compatibility but unused
    long long repeat = 2000000000LL;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg == "--impl=optimized") impl = "optimized";
        else if (arg == "--impl=original") impl = "original";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoll(arg.substr(9));
    }

    TankData tank = create_tank();
    state_type x(3, 0.1), dxdt(3);
    double t = 1.0;

    if (mode == "correct") {
        step(x, dxdt, t, tank);
        double sum = 0; for (double v : dxdt) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (long long i = 0; i < repeat; ++i) {
            step(x, dxdt, t, tank);
            dummy += dxdt[0];
            x[0] += 1e-9; // prevent constant folding
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 42) std::cerr << "";
    }
    return 0;
}
