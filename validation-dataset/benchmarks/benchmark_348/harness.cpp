#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

const int* getValueTypeList(int);

constexpr int LAST_VALUETYPE = 32;

// Worker that repeatedly calls getValueTypeList and sums the results
static void worker(size_t iterations, std::atomic<size_t>& out_sum) {
    size_t local = 0;
    for (size_t i = 0; i < iterations; ++i) {
        int vt = static_cast<int>(i % LAST_VALUETYPE);
        const int* p = getValueTypeList(vt);
        local += *p;
    }
    out_sum += local;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    size_t iterations = 1000000;   // per-thread iterations
    int threads = 8;
    int repeat = 20; // enough iterations to keep the locked version running >10s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--iterations=", 0) == 0) iterations = std::stoull(arg.substr(13));
        else if (arg.rfind("--threads=", 0) == 0) threads = std::stoi(arg.substr(10));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        iterations = 1000;
        repeat = 1;
        threads = 2;
    }

    if (mode == "perf") {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            std::atomic<size_t> sum{0};
            std::vector<std::thread> ths;
            ths.reserve(threads);
            for (int t = 0; t < threads; ++t)
                ths.emplace_back(worker, iterations, std::ref(sum));
            for (auto& th : ths) th.join();
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    } else {
        std::atomic<size_t> sum{0};
        std::vector<std::thread> ths;
        ths.reserve(threads);
        for (int t = 0; t < threads; ++t)
            ths.emplace_back(worker, iterations, std::ref(sum));
        for (auto& th : ths) th.join();
        std::cout << sum.load() << std::endl;
    }
    return 0;
}
