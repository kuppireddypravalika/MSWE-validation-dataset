#include "streamer.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int threads = 6;
    int iters = 6000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
        else if (arg.rfind("--iters=",0)==0) iters = std::stoi(arg.substr(8));
    }

    MockBio bio(true);
    Streamer streamer(&bio);
    const uint32_t timeout = 300; // microseconds for cansend simulation

    if (mode == "correct") {
        for (int i = 0; i < iters; ++i) {
            streamer.tick_send(timeout);
        }
        std::cout << streamer.sent << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    std::vector<std::thread> workers;
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&, timeout]() {
            for (int i = 0; i < iters; ++i) {
                streamer.tick_send(timeout);
            }
        });
    }
    for (auto& th : workers) th.join();
    auto end = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
    return 0;
}
