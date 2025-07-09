#include <chrono>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

// Target function implemented by either version
double ellint_algorithm(const std::vector<double>& data, int inner);

std::vector<double> generate_data(std::size_t n)
{
    std::vector<double> v(n);
    for (std::size_t i = 0; i < n; ++i)
        v[i] = 0.5 + 0.5 * std::sin(static_cast<double>(i));
    return v;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    std::size_t size = 1000;  // number of elements in data vector
    int inner = 800;          // inner loop count controlling workload
    int repeat = 1;           // number of repetitions for perf measurement

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--inner=",0) == 0) inner = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto data = generate_data(size);

    if (mode == "correct") {
        double res = ellint_algorithm(data, inner);
        std::cout << res << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            ellint_algorithm(data, inner);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
