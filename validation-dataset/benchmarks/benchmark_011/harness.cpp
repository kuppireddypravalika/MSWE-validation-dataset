#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

float median(std::vector<float> vals);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 800000;
    int window_size = 180;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        } else if (arg.rfind("--window_size=", 0) == 0) {
            window_size = std::stoi(arg.substr(14));
        }
    }

    // Prepare random data
    std::vector<std::vector<float>> data(repeat, std::vector<float>(window_size));
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> dist(0.0f, 1000.0f);
    for (int i = 0; i < repeat; ++i) {
        for (int j = 0; j < window_size; ++j) {
            data[i][j] = dist(rng);
        }
    }

    if (mode == "correct") {
        float result = median(data[0]);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            median(data[i]);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
