#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace emp {
    double run(const double* data, std::size_t len, double exponent);
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";      // perf or correct
    std::size_t size = 100000;      // array length
    int repeat = 80;                // number of iterations
    double exponent = 1.7;          // exponent value

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoull(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--exp=", 0) == 0) exponent = std::stod(arg.substr(6));
    }

    // generate data
    std::vector<double> data(size);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(1.0, 8.0);
    for (auto& v : data) v = dist(rng);

    if (mode == "correct") {
        double result = emp::run(data.data(), size, exponent);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) emp::run(data.data(), size, exponent);
        auto t2 = clock::now();

        std::cout << "Time:  "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
