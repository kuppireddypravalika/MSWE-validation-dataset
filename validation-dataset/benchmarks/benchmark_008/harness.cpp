#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

float compute_median(std::vector<float> data);

std::vector<float> generate_data(size_t size) {
    std::vector<float> data(size);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 100.0f);
    for (auto& val : data) {
        val = dist(gen);
    }
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t data_size = 800000;
    int repeat = 100;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            data_size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto data = generate_data(data_size);

    if (mode == "correct") {
        float result = compute_median(data);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            compute_median(data);
        }
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
