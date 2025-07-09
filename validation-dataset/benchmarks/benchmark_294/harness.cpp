#include "common.hpp"
#include <chrono>
#include <iostream>
#include <string>

void consume(Vehicle* m_vehicle, const ServoPosition* msg);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    long repeat = 1000000000; // default iterations

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stol(arg.substr(9));
    }

    UUV vehicle;
    Vehicle* m_vehicle = &vehicle;
    ServoPosition msgs[4];
    for (int i = 0; i < 4; ++i) { msgs[i].id = i; msgs[i].value = 0.1f * i; }

    if (mode == "correct") {
        consume(m_vehicle, &msgs[0]);
        float sum = 0.0f; for (float v : vehicle.fins) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (long i = 0; i < repeat; ++i) {
            consume(m_vehicle, &msgs[i & 3]);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
