#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// declaration
void topk(const std::vector<float>& input, size_t row, size_t col, size_t k,
          std::vector<float>& output, std::vector<size_t>& indices);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t row = 150;
    size_t col = 1000;
    size_t k = 100;
    int repeat = 3000;  // ensure runtime >10s for original
    std::string impl = "original"; // parsed for compatibility

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--row=", 0) == 0)
            row = static_cast<size_t>(std::stoul(arg.substr(6)));
        else if (arg.rfind("--col=", 0) == 0)
            col = static_cast<size_t>(std::stoul(arg.substr(6)));
        else if (arg.rfind("--k=", 0) == 0)
            k = static_cast<size_t>(std::stoul(arg.substr(4)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--impl=", 0) == 0)
            impl = arg.substr(7);
    }

    std::vector<float> base(row * col);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1000.0f);
    for (size_t i = 0; i < base.size(); ++i) base[i] = dist(gen);

    std::vector<float> output;
    std::vector<size_t> indices;

    if (mode == "correct") {
        topk(base, row, col, k, output, indices);
        double sum = 0.0;
        for (float v : output) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            topk(base, row, col, k, output, indices);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
