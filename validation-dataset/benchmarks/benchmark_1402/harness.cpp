#include "tensor.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>

void topk(const Tensor<float>& input, Tensor<float>& output, Tensor<int64_t>& indices, std::size_t k);

Tensor<float> make_input(std::size_t rows, std::size_t cols) {
    Tensor<float> t(rows, cols);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (auto& v : t.data) v = dist(gen);
    return t;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t rows = 2048;
    std::size_t cols = 1024;
    std::size_t k = 10;
    int repeat = 1000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--rows=",0) == 0) rows = std::stoul(arg.substr(7));
        else if (arg.rfind("--cols=",0) == 0) cols = std::stoul(arg.substr(7));
        else if (arg.rfind("--k=",0) == 0) k = std::stoul(arg.substr(4));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    Tensor<float> input = make_input(rows, cols);
    Tensor<float> output(rows, k);
    Tensor<int64_t> indices(rows, k);

    if (mode == "correct") {
        topk(input, output, indices, k);
        long long sum = 0;
        for (float v : output.data) sum += static_cast<long long>(v);
        for (int64_t v : indices.data) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            topk(input, output, indices, k);
            dummy += static_cast<long long>(output(0,0));
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 42) std::cerr << "";
    }
    return 0;
}
