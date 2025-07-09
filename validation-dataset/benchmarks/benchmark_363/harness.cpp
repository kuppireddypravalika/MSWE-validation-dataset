#include <chrono>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <cstddef>

double parse_numbers(const char* text, size_t count);

std::string make_data(size_t n) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::ostringstream oss;
    for (size_t i = 0; i < n; ++i) {
        oss << dist(gen);
        if (i + 1 < n) oss << ' ';
    }
    return oss.str();
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t count = 1000;
    int repeat = 70000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--count=", 0) == 0) count = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::string data = make_data(count);

    if (mode == "correct") {
        double sum = parse_numbers(data.c_str(), count);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            parse_numbers(data.c_str(), count);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
}
