#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// function declaration implemented in original.cpp / optimized.cpp
void convert_half_to_float(std::vector<float>& dst, const std::vector<uint16_t>& src);

std::vector<uint16_t> make_input(std::size_t n) {
    std::vector<uint16_t> data(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 65535);
    for (std::size_t i = 0; i < n; ++i) data[i] = static_cast<uint16_t>(dist(gen));
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 1000000; // number of elements
    int repeat = 25000;         // number of iterations (>=10s on default runner)

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoull(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<uint16_t> input = make_input(size);
    std::vector<float> output(size);

    if (mode == "correct") {
        convert_half_to_float(output, input);
        double sum = 0.0;
        for (float v : output) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        double dummy = 0.0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            convert_half_to_float(output, input);
            dummy += output[i % output.size()];
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789.0) std::cerr << "";
    }
    return 0;
}
