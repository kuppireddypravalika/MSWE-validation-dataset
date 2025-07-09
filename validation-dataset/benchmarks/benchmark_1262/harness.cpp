#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void* my_memcpy(void* dest, const void* src, size_t n);

std::vector<char> make_buffer(std::size_t n) {
    std::vector<char> buf(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 255);
    for (auto& b : buf) b = static_cast<char>(dist(gen));
    return buf;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 1024 * 1024; // 1MB
    int repeat = 24000;            // ensures >5s for original

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto src = make_buffer(size);
    std::vector<char> dest(size);

    if (mode == "correct") {
        my_memcpy(dest.data(), src.data(), size);
        unsigned long long sum = 0;
        for (unsigned char c : dest) sum += c;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        volatile unsigned char dummy = 0;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            my_memcpy(dest.data(), src.data(), size);
            dummy += dest[0];
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123) std::cerr << "";
    }
    return 0;
}
