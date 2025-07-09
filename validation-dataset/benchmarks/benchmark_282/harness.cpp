#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Kernel { bool fastMath; };

double process(std::vector<float>& data);

std::vector<float> create_data(size_t n) {
    std::vector<float> data(n);
    for (size_t i = 0; i < n; ++i)
        data[i] = static_cast<float>(i % 100) / 100.0f;
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 1500000; // elements
    int repeat = 110;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoull(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto base = create_data(size);

    if (mode == "correct") {
        auto data = base;
        double sum = process(data);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto data1 = base;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            process(data1);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
