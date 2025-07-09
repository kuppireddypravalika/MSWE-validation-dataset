#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

long long process(std::vector<int> inputs);

std::vector<int> make_data(size_t n) {
    std::vector<int> v(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 1000);
    for (auto &x : v) x = dist(gen);
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 10000;
    int repeat = 250000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto data = make_data(size);

    if (mode == "correct") {
        long long sum = process(data);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            process(data);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
