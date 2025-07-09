#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <limits>

unsigned long long count_nan(const std::vector<float>& data, int repeat);

std::vector<float> create_data(size_t n) {
    std::vector<float> data(n);
    std::mt19937 rng(123);
    std::uniform_real_distribution<float> dist(-1e4, 1e4);
    for (size_t i = 0; i < n; ++i) {
        if (i % 10 == 0)
            data[i] = std::numeric_limits<float>::quiet_NaN();
        else
            data[i] = dist(rng);
    }
    return data;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    size_t size = 1000000;
    int repeat = 7000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<float> data = create_data(size);

    if (mode == "correct") {
        unsigned long long result = count_nan(data, repeat);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        count_nan(data, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
