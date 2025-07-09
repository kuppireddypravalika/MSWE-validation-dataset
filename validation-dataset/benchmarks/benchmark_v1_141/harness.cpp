#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

// function under test
void relu_forward(const float* bottom, float* top, int count, float negative_slope);

static std::vector<float> init_data(int count) {
    std::vector<float> data(count);
    for (int i = 0; i < count; ++i) {
        data[i] = static_cast<float>((i % 100) - 50) / 50.0f; // values in [-1,1]
    }
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    // Tune workload so the original implementation runs for ~10s
    int count = 8000000; // number of elements
    int repeat = 4000;
    int threads = std::min(4, omp_get_max_threads());
    float negative_slope = 0.1f;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--count=",0)==0) count = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
        else if (arg.rfind("--slope=",0)==0) negative_slope = std::stof(arg.substr(8));
    }

    omp_set_num_threads(threads);

    std::vector<float> bottom = init_data(count);
    std::vector<float> top(count);

    if (mode == "correct") {
        relu_forward(bottom.data(), top.data(), count, negative_slope);
        double sum = 0.0;
        for (float v : top) sum += v;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) relu_forward(bottom.data(), top.data(), count, negative_slope);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
