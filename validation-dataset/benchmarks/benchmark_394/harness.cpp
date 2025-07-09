#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Context { bool skipUniformRegions = false; };

void initialize(Context& ctx);
unsigned long process(const Context& ctx, const std::vector<int>& data);

std::vector<int> generate_data(size_t size) {
    std::vector<int> data(size);
    std::mt19937 rng(0);
    std::uniform_int_distribution<int> dist(1, 100);
    int current = dist(rng);
    const size_t block = 256;
    for (size_t i = 0; i < size; ++i) {
        if (i % block == 0) current = dist(rng);
        data[i] = current;
    }
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 1'000'000; // number of elements
    int repeat = 1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<int> data = generate_data(size);
    Context ctx;
    initialize(ctx);

    if (mode == "correct") {
        unsigned long sum = process(ctx, data);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) process(ctx, data);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
