#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// function under test
void compute_distance_matrix(const std::vector<std::vector<double>>&,
                             std::vector<std::vector<double>>&);

static std::vector<std::vector<double>> make_data(std::size_t n, std::size_t dim) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<std::vector<double>> data(n, std::vector<double>(dim));
    for (auto& v : data)
        for (auto& x : v)
            x = dist(gen);
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t sets = 800;
    std::size_t dim = 64;
    int repeat = 600;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--sets=", 0) == 0)
            sets = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--dim=", 0) == 0)
            dim = static_cast<std::size_t>(std::stoul(arg.substr(6)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto data = make_data(sets, dim);
    std::vector<std::vector<double>> dist(sets, std::vector<double>(sets));

    if (mode == "correct") {
        compute_distance_matrix(data, dist);
        double sum = 0.0;
        for (const auto& row : dist)
            for (double v : row)
                sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto start = clock::now();
        for (int r = 0; r < repeat; ++r)
            compute_distance_matrix(data, dist);
        auto end = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
    }
    return 0;
}
