#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

extern "C" uint64_t vbsl(const uint64_t*, const uint64_t*, const uint64_t*, uint64_t*, size_t);

std::vector<uint64_t> generate_data(size_t n, uint64_t seed) {
    std::vector<uint64_t> v(n);
    std::mt19937_64 gen(seed);
    for (auto &x : v) x = gen();
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 1 << 20; // elements
    int repeat = 5200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoull(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto mask = generate_data(size, 1);
    auto vtrue = generate_data(size, 2);
    auto vfalse = generate_data(size, 3);
    std::vector<uint64_t> out(size);

    if (mode == "correct") {
        uint64_t sum = vbsl(mask.data(), vtrue.data(), vfalse.data(), out.data(), size);
        std::cout << sum << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
        uint64_t acc = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            acc += vbsl(mask.data(), vtrue.data(), vfalse.data(), out.data(), size);
        auto t2 = clock::now();
        std::cerr << "ignore:" << acc << "\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    return 0;
}
