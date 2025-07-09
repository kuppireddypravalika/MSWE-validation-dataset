#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int bitscanforward(int source);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 1000000; // number of integers
    int repeat = 1000;           // number of repetitions

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--size=", 0) == 0) {
            size = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        }
    }

    std::vector<int> data(size);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 0x7fffffff);
    for (std::size_t i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }

    if (mode == "correct") {
        long long sum = 0;
        for (int value : data) sum += bitscanforward(value);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            for (int value : data) dummy += bitscanforward(value);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
