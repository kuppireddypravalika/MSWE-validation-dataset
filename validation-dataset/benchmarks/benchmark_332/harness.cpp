#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

float calculate_temperature(uint32_t adc);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long repeat = 200000000; // default iterations
    uint32_t adc = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stol(arg.substr(9));
        } else if (arg.rfind("--adc=", 0) == 0) {
            adc = static_cast<uint32_t>(std::stoul(arg.substr(6)));
        }
    }

    if (mode == "correct") {
        float t = calculate_temperature(adc);
        std::cout << t << std::endl;
    } else {
        volatile float sum = 0.0f;
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (long i = 0; i < repeat; ++i) {
            uint32_t cur = adc + static_cast<uint32_t>(i % 1024);
            sum += calculate_temperature(cur);
        }
        auto t2 = clock::now();
        if (sum == 0.123f) std::cerr << ""; // prevent optimization
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
