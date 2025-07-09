#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Symbol {
    uint64_t value;
    uint32_t id;
};

// Forward declarations
void sort_symbol_indexes(std::vector<uint32_t>& indexes,
                         const std::vector<Symbol>& symbols);

std::vector<Symbol> generate_symbols(size_t n) {
    std::vector<Symbol> syms(n);
    for (size_t i = 0; i < n; ++i) {
        syms[i].value = i; // monotonically increasing values
        syms[i].id = static_cast<uint32_t>(i);
    }
    return syms;
}

std::vector<uint32_t> generate_near_sorted_indexes(size_t n, int shuffles) {
    std::vector<uint32_t> idx(n);
    for (size_t i = 0; i < n; ++i)
        idx[i] = static_cast<uint32_t>(i);

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist(0, n - 1);
    for (int i = 0; i < shuffles; ++i)
        std::swap(idx[dist(rng)], idx[dist(rng)]);
    return idx;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 10000;
    size_t size = 50000;
    int shuffles = 10; // how many swaps to perform

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--shuffles=", 0) == 0)
            shuffles = std::stoi(arg.substr(11));
    }

    auto symbols = generate_symbols(size);
    auto base_indexes = generate_near_sorted_indexes(size, shuffles);

    if (mode == "correct") {
        auto indexes = base_indexes;
        sort_symbol_indexes(indexes, symbols);
        // simple checksum so outputs are comparable
        uint64_t sum = 0;
        for (auto v : indexes) sum += v;
        std::cout << sum << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            auto idx = base_indexes;
            sort_symbol_indexes(idx, symbols);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
