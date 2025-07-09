#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>

extern "C" void compute(const float*, const float*, const float*, float*, size_t);

std::vector<float> make_data(size_t n) {
    std::vector<float> v(n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    for (auto &x : v) x = dist(gen);
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 65536;
    int repeat = 350000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<float> a = make_data(size);
    std::vector<float> b = make_data(size);
    std::vector<float> c = make_data(size);
    std::vector<float> out(size);

    if (mode == "correct") {
        compute(a.data(), b.data(), c.data(), out.data(), size);

        float sum = 0.f;
        for (float v : out) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            compute(a.data(), b.data(), c.data(), out.data(), size);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
