#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>

unsigned long long flatten_sum(const std::vector<std::vector<int>>& data, int repeat);

std::vector<std::vector<int>> create_data(size_t outer, size_t inner) {
    std::vector<std::vector<int>> data;
    data.reserve(outer);
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, 100);
    for (size_t i = 0; i < outer; ++i) {
        std::vector<int> row(inner);
        for (size_t j = 0; j < inner; ++j) {
            row[j] = dist(rng);
        }
        data.push_back(std::move(row));
    }
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t outer = 200;  // number of inner sequences
    size_t inner = 1000; // elements per inner sequence
    int repeat = 25000;  // workload repetitions

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--outer=", 0) == 0) outer = std::stoul(arg.substr(8));
        else if (arg.rfind("--inner=", 0) == 0) inner = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<std::vector<int>> data = create_data(outer, inner);

    if (mode == "correct") {
        unsigned long long result = flatten_sum(data, repeat);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        flatten_sum(data, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
