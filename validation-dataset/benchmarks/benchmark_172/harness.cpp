#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct STFT;
extern "C" STFT* create(uint32_t size);
extern "C" void destroy(STFT* stft);
extern "C" double run(STFT* stft, const std::vector<double>& in);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    uint32_t size = 2048;
    int repeat = 200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = static_cast<uint32_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    std::vector<double> input(size);
    for (uint32_t i = 0; i < size; ++i)
        input[i] = dist(gen);

    STFT* stft = create(size);

    if (mode == "correct") {
        double res = run(stft, input);
        std::cout << res << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            run(stft, input);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    destroy(stft);
    return 0;
}

